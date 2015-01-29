/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <libusermetricsoutput/SyncedUserMetricsStore.h>
#include <libusermetricsoutput/SyncedUserData.h>
#include <libusermetricsoutput/SyncedDataSource.h>

#include <libusermetricscommon/DataSourceInterface.h>
#include <libusermetricscommon/DBusPaths.h>

using namespace com;
using namespace UserMetricsCommon;
using namespace UserMetricsOutput;

SyncedUserMetricsStore::SyncedUserMetricsStore(
		const QDBusConnection &dbusConnection, QObject *parent) :
		UserMetricsStore(parent), m_interface(DBusPaths::serviceName(),
				DBusPaths::userMetrics(), dbusConnection) {
	QTimer::singleShot(0, this, SLOT(sync()));
}

SyncedUserMetricsStore::~SyncedUserMetricsStore() {
}

void SyncedUserMetricsStore::sync() {
	QDBusConnectionInterface* interface = m_interface.connection().interface();
	if (!interface->isServiceRegistered(DBusPaths::serviceName())) {
		QDBusReply<void> reply(
				interface->startService(DBusPaths::serviceName()));
	}

	connect(&m_interface,
	SIGNAL(dataSourceAdded(const QDBusObjectPath &)), this,
	SLOT(addDataSource(const QDBusObjectPath &)));

	connect(&m_interface,
	SIGNAL(dataSourceRemoved(const QDBusObjectPath &)), this,
	SLOT(removeDataSource(const QDBusObjectPath &)));

	connect(&m_interface,
	SIGNAL(userDataAdded(const QString &, const QDBusObjectPath &)), this,
	SLOT(addUserData(const QString &, const QDBusObjectPath &)));

	connect(&m_interface,
	SIGNAL(userDataRemoved(const QString &, const QDBusObjectPath &)), this,
	SLOT(removeUserData(const QString &, const QDBusObjectPath &)));

	for (const QDBusObjectPath &path : m_interface.dataSources()) {
		addDataSource(path);
	}

	QSharedPointer<canonical::usermetrics::UserData> systemData;

	for (const QDBusObjectPath &path : m_interface.userDatas()) {

		QSharedPointer<canonical::usermetrics::UserData> userData(
				new canonical::usermetrics::UserData(DBusPaths::serviceName(),
						path.path(), m_interface.connection()));

		QString username(userData->username());
		if (username == "") {
			systemData = userData;
			continue;
		}
		insert(username, UserDataPtr(new SyncedUserData(*this, userData)));
	}

	// if we have system data we must attach it to each of the user datas
	if (!systemData.isNull()) {
		attachSystemData(systemData);
	}

	connectionEstablished();
}

void SyncedUserMetricsStore::attachSystemData(
		QSharedPointer<canonical::usermetrics::UserData> systemData) {
	for (UserDataPtr userData : m_userData.values()) {
		SyncedUserData *syncedData = qobject_cast<SyncedUserData *>(
				userData.data());
		syncedData->attachUserData(systemData);
	}
}

void SyncedUserMetricsStore::addUserData(const QString &username,
		const QDBusObjectPath &path) {

	QSharedPointer<canonical::usermetrics::UserData> userData(
			new canonical::usermetrics::UserData(DBusPaths::serviceName(),
					path.path(), m_interface.connection()));

	// if we're adding system data for the first time, we must attach it
	// to all of the user datas
	if (username == "") {
		attachSystemData(userData);
	} else {
		insert(username, UserDataPtr(new SyncedUserData(*this, userData)));
	}
}

void SyncedUserMetricsStore::removeUserData(const QString &username,
		const QDBusObjectPath &path) {
	Q_UNUSED(path);

	//FIXME Handle system user data being removed
	if (username == "") {
		return;
	}

	m_dataSources.remove(username);
}

void SyncedUserMetricsStore::addDataSource(const QDBusObjectPath &path) {

	QSharedPointer<canonical::usermetrics::DataSource> dataSource(
			new canonical::usermetrics::DataSource(DBusPaths::serviceName(),
					path.path(), m_interface.connection()));

	insert(path.path(),
			DataSourcePtr(new SyncedDataSource(dataSource)));
}

void SyncedUserMetricsStore::removeDataSource(const QDBusObjectPath &path) {
	m_userData.remove(path.path());
}
