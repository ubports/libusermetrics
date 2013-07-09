/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <stdexcept>

#include <usermetricsservice/DBusDataSource.h>
#include <usermetricsservice/DBusUserMetrics.h>
#include <usermetricsservice/DBusUserData.h>
#include <usermetricsservice/UserMetricsAdaptor.h>
#include <usermetricsservice/database/DataSource.h>
#include <usermetricsservice/database/UserData.h>
#include <usermetricsservice/database/DataSet.h>
#include <libusermetricscommon/DateFactory.h>
#include <libusermetricscommon/DBusPaths.h>
#include <libusermetricscommon/Localisation.h>

#include <QDjango.h>
#include <QDjangoQuerySet.h>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

DBusUserMetrics::DBusUserMetrics(QDBusConnection &dbusConnection,
		QSharedPointer<DateFactory> dateFactory, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new UserMetricsAdaptor(this)), m_dateFactory(dateFactory) {

	// Database setup
	QDjango::registerModel<DataSource>().createTable();
	QDjango::registerModel<UserData>().createTable();
	QDjango::registerModel<DataSet>().createTable();

	// DBus setup

	if (!m_dbusConnection.registerObject(DBusPaths::userMetrics(), this)) {
		throw logic_error(_("Unable to register user metrics object on DBus"));
	}

	syncDatabase();
}

DBusUserMetrics::~DBusUserMetrics() {
	m_dbusConnection.unregisterObject(DBusPaths::userMetrics());
}

QList<QDBusObjectPath> DBusUserMetrics::dataSources() const {
	QList<QDBusObjectPath> dataSources;
	for (DBusDataSourcePtr dataSource : m_dataSources.values()) {
		dataSources << QDBusObjectPath(dataSource->path());
	}
	return dataSources;
}

void DBusUserMetrics::syncDatabase() {
	{
		QSet<int> dataSourceNames;
		QDjangoQuerySet<DataSource> query;
		for (const DataSource &dataSource : query) {
			const int id(dataSource.id());
			dataSourceNames << id;
			// if we don't have a local cache
			if (!m_dataSources.contains(id)) {
				DBusDataSourcePtr dbusDataSource(
						new DBusDataSource(id, dataSource.name(),
								m_dbusConnection));
				m_dataSources.insert(id, dbusDataSource);
				m_adaptor->dataSourceAdded(dbusDataSource->name(),
						QDBusObjectPath(dbusDataSource->path()));
			}
		}
		// remove any cached references to deleted sources
		QSet<int> cachedDataSourceNames(
				QSet<int>::fromList(m_dataSources.keys()));
		QSet<int> &toRemove(cachedDataSourceNames.subtract(dataSourceNames));
		for (int id : toRemove) {
			DBusDataSourcePtr dataSource(m_dataSources.take(id));
			m_adaptor->dataSourceRemoved(dataSource->name(),
					QDBusObjectPath(dataSource->path()));
		}
	}

	{
		QSet<int> usernames;
		QDjangoQuerySet<UserData> query;
		for (const UserData &userData : query) {
			const int id(userData.id());
			usernames << id;
			// if we don't have a local cache
			if (!m_userData.contains(id)) {
				DBusUserDataPtr dbusUserData(
						new DBusUserData(id, userData.username(), *this,
								m_dbusConnection, m_dateFactory));
				m_userData.insert(id, dbusUserData);
				m_adaptor->userDataAdded(dbusUserData->username(),
						QDBusObjectPath(dbusUserData->path()));
			}
		}
		// remove any cached references to deleted sources
		QSet<int> cachedUsernames(QSet<int>::fromList(m_userData.keys()));
		QSet<int> &toRemove(cachedUsernames.subtract(usernames));
		for (int id : toRemove) {
			DBusUserDataPtr userData(m_userData.take(id));
			m_adaptor->userDataRemoved(userData->username(),
					QDBusObjectPath(userData->path()));
		}
	}
}

QDBusObjectPath DBusUserMetrics::createDataSource(const QString &name,
		const QString &formatString, const QString &emptyDataString,
		const QString &textDomain) {
	QDjangoQuerySet<DataSource> dataSourcesQuery;
	QDjangoQuerySet<DataSource> query(
			dataSourcesQuery.filter(
					QDjangoWhere("name", QDjangoWhere::Equals, name)));

	if (query.size() == -1) {
		throw logic_error(_("Data source query failed"));
	}

	DataSource dataSource;

	if (query.size() == 0) {
		dataSource.setName(name);
		dataSource.setFormatString(formatString);
		dataSource.setEmptyDataString(emptyDataString);
		dataSource.setTextDomain(textDomain);

		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}

		syncDatabase();
	} else {
		query.at(0, &dataSource);
		const DBusDataSourcePtr dbusDataSource(
				*m_dataSources.constFind(dataSource.id()));
		if (dataSource.formatString() != formatString) {
			dbusDataSource->setFormatString(formatString);
		}
		if (dataSource.emptyDataString() != emptyDataString) {
			dbusDataSource->setEmptyDataString(emptyDataString);
		}
		if (dataSource.textDomain() != textDomain) {
			dbusDataSource->setTextDomain(textDomain);
		}
	}

	return QDBusObjectPath((*m_dataSources.constFind(dataSource.id()))->path());
}

QList<QDBusObjectPath> DBusUserMetrics::userDatas() const {
	QList<QDBusObjectPath> userDatas;
	for (DBusUserDataPtr userData : m_userData.values()) {
		userDatas << QDBusObjectPath(userData->path());
	}
	return userDatas;
}

QDBusObjectPath DBusUserMetrics::createUserData(const QString &username) {
	QDjangoQuerySet<UserData> query(
			QDjangoQuerySet<UserData>().filter(
					QDjangoWhere("username", QDjangoWhere::Equals, username)));

	if (query.size() == -1) {
		throw logic_error(_("User data query failed"));
	}

	UserData userData;

	if (query.size() == 0) {
		userData.setUsername(username);

		if (!userData.save()) {
			throw logic_error(_("Could not save user data"));
		}

		syncDatabase();
	} else {
		query.at(0, &userData);
	}

	return QDBusObjectPath((*m_userData.constFind(userData.id()))->path());
}

DBusDataSourcePtr DBusUserMetrics::dataSource(const QString &name) const {
	DataSource dataSource;
	DataSource::findByName(name, &dataSource);

	return m_dataSources.value(dataSource.id());
}

DBusUserDataPtr DBusUserMetrics::userData(const QString &username) const {
	UserData userData;
	UserData::findByName(username, &userData);

	return m_userData.value(userData.id());
}
