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

#include <usermetricsservice/DBusUserMetrics.h>
#include <usermetricsservice/UserMetricsAdaptor.h>
#include <usermetricsservice/DataSource.h>
#include <usermetricsservice/DBusDataSource.h>

#include <QDjango.h>
#include <QDjangoQuerySet.h>
#include <QSqlDatabase>

#include <QtCore/QDebug>

using namespace UserMetricsService;

DBusUserMetrics::DBusUserMetrics() :
		m_adaptor(new UserMetricsAdaptor(this)) {

	// DBus setup
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.registerService("com.canonical.UserMetrics");
	connection.registerObject("/com/canonical/UserMetrics", this);

	// Database setup
	QDjango::registerModel<DataSource>();
	QDjango::createTables();

	syncDatabase();
}

DBusUserMetrics::~DBusUserMetrics() {
}

QList<QDBusObjectPath> DBusUserMetrics::dataSources() const {
	return QList<QDBusObjectPath>();
}

void DBusUserMetrics::syncDatabase() {
	QSet<QString> dataSourceNames;
	QDjangoQuerySet<DataSource> dataSources;
	for (const DataSource &dataSource : dataSources) {
		const QString &name(dataSource.name());
		dataSourceNames << name;
		// if we don't have a local cache
		if (!m_dataSources.contains(name)) {
			DBusDataSourcePtr dbusDataSource(new DBusDataSource(name));
			m_dataSources.insert(dataSource.name(), dbusDataSource);
		}
	}
	// remove any cached references to deleted sources
	QSet<QString> cachedDataSourceNames(
			QSet<QString>::fromList(m_dataSources.keys()));
	QSet<QString> &toRemove(cachedDataSourceNames.subtract(dataSourceNames));
	for (QString name : toRemove) {
		m_dataSources.remove(name);
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
		dataSource.save();

		syncDatabase();
	}

	return QDBusObjectPath((*m_dataSources.constFind(name))->path());
}
