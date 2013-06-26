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

#include <usermetricsservice/DBusDataSource.h>
#include <usermetricsservice/DBusUserMetrics.h>
#include <usermetricsservice/DBusUserData.h>
#include <usermetricsservice/UserMetricsAdaptor.h>
#include <usermetricsservice/database/DataSource.h>
#include <usermetricsservice/database/UserData.h>
#include <usermetricsservice/database/DataSet.h>

#include <QDjango.h>
#include <QDjangoQuerySet.h>

#include <QtCore/QDebug>

using namespace UserMetricsService;

DBusUserMetrics::DBusUserMetrics(QDBusConnection &dbusConnection,
		QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new UserMetricsAdaptor(this)) {

	// DBus setup
	m_dbusConnection.registerService("com.canonical.UserMetrics");
	m_dbusConnection.registerObject("/com/canonical/UserMetrics", this);

	// Database setup
	QDjango::registerModel<DataSource>();
	QDjango::registerModel<UserData>();
	QDjango::registerModel<DataSet>();

//	QDjango::dropTables();
	QDjango::createTables();

	syncDatabase();
}

DBusUserMetrics::~DBusUserMetrics() {
	m_dbusConnection.unregisterObject("/com/canonical/UserMetrics");
	m_dbusConnection.unregisterService("com.canonical.UserMetrics");
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
		QSet<QString> dataSourceNames;
		QDjangoQuerySet<DataSource> query;
		for (const DataSource &dataSource : query) {
			const QString &name(dataSource.name());
			dataSourceNames << name;
			// if we don't have a local cache
			if (!m_dataSources.contains(name)) {
				DBusDataSourcePtr dbusDataSource(
						new DBusDataSource(name, m_dbusConnection));
				m_dataSources.insert(name, dbusDataSource);
			}
		}
		// remove any cached references to deleted sources
		QSet<QString> cachedDataSourceNames(
				QSet<QString>::fromList(m_dataSources.keys()));
		QSet<QString> &toRemove(
				cachedDataSourceNames.subtract(dataSourceNames));
		for (QString name : toRemove) {
			m_dataSources.remove(name);
		}
	}

	{
		QSet<QString> usernames;
		QDjangoQuerySet<UserData> query;
		for (const UserData &userData : query) {
			const QString &username(userData.username());
			usernames << username;
			// if we don't have a local cache
			if (!m_userData.contains(username)) {
				DBusUserDataPtr dbusUserData(
						new DBusUserData(username, *this, m_dbusConnection));
				m_userData.insert(userData.username(), dbusUserData);
			}
		}
		// remove any cached references to deleted sources
		QSet<QString> cachedUsernames(
				QSet<QString>::fromList(m_userData.keys()));
		QSet<QString> &toRemove(cachedUsernames.subtract(usernames));
		for (QString name : toRemove) {
			m_userData.remove(name);
		}
	}
}

QDBusObjectPath DBusUserMetrics::createDataSource(const QString &name,
		const QString &formatString) {
	qDebug() << "createDataSource(" << name << "," << formatString << ")";

	QDjangoQuerySet<DataSource> dataSourcesQuery;
	QDjangoQuerySet<DataSource> dataSourceQuery(
			dataSourcesQuery.filter(
					QDjangoWhere("name", QDjangoWhere::Equals, name)));

	if (dataSourceQuery.size() == 0) {
		DataSource dataSource;
		dataSource.setName(name);
		dataSource.setFormatString(formatString);
		Q_ASSERT(dataSource.save());

		syncDatabase();
	}

	return QDBusObjectPath((*m_dataSources.constFind(name))->path());
}

QList<QDBusObjectPath> DBusUserMetrics::userData() const {
	QList<QDBusObjectPath> userDatas;
	for (DBusUserDataPtr userData : m_userData.values()) {
		userDatas << QDBusObjectPath(userData->path());
	}
	return userDatas;
}

QDBusObjectPath DBusUserMetrics::createUserData(const QString &username) {
	qDebug() << "createUserData(" << username << ")";

	QDjangoQuerySet<UserData> query(
			QDjangoQuerySet<UserData>().filter(
					QDjangoWhere("username", QDjangoWhere::Equals, username)));

	if (query.size() == 0) {
		UserData userData;
		userData.setUsername(username);
		Q_ASSERT(userData.save());

		syncDatabase();
	}

	return QDBusObjectPath((*m_userData.constFind(username))->path());
}

DBusDataSourcePtr DBusUserMetrics::dataSource(const QString &name) const {
	return m_dataSources.value(name);
}

DBusUserDataPtr DBusUserMetrics::userData(const QString &username) const {
	return m_userData.value(username);
}
