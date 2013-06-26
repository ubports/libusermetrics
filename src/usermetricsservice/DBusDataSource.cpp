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

#include <usermetricsservice/database/DataSource.h>
#include <usermetricsservice/DBusDataSource.h>
#include <usermetricsservice/DataSourceAdaptor.h>

#include <QDjangoQuerySet.h>

using namespace UserMetricsService;

DBusDataSource::DBusDataSource(const QString &name,
		QDBusConnection &dbusConnection, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new DataSourceAdaptor(this)), m_name(name), m_path(
				QString("/com/canonical/UserMetrics/DataSource/%1").arg(m_name)) {

	// DBus setup
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.registerObject(m_path, this);
}

DBusDataSource::~DBusDataSource() {
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.unregisterObject(m_path);
}

QString DBusDataSource::path() const {
	return m_path;
}

QString DBusDataSource::name() const {
	return m_name;
}

QString DBusDataSource::formatString() const {
	DataSource dataSource;
	DataSource::findByName(m_name, &dataSource);
	return dataSource.formatString();
}

void DBusDataSource::setFormatString(const QString &formatString) {
	DataSource dataSource;
	DataSource::findByName(m_name, &dataSource);
	dataSource.setFormatString(formatString);
	dataSource.save();
}
