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
#include <libusermetricscommon/UserDataInterface.h>
#include <libusermetricscommon/DBusPaths.h>

using namespace com;
using namespace UserMetricsCommon;
using namespace UserMetricsOutput;

SyncedUserMetricsStore::SyncedUserMetricsStore(
		const QDBusConnection &dbusConnection, QObject *parent) :
		UserMetricsStore(parent), m_dbusConnection(dbusConnection), m_interface(
				DBusPaths::serviceName(), DBusPaths::userMetrics(),
				dbusConnection) {
	qDebug() << "SyncedUserMetricsStore.new";
	for (const QDBusObjectPath &path : m_interface.userDatas()) {
		qDebug() << "SyncedUserMetricsStore.each(" << path.path();

		canonical::usermetrics::UserData userData(DBusPaths::serviceName(),
				path.path(), m_dbusConnection);
		QString username(userData.username());
		find(username);
	}
}

SyncedUserMetricsStore::~SyncedUserMetricsStore() {
}
