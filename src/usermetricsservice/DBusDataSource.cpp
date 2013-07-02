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
#include <libusermetricscommon/DBusPaths.h>

#include <QDjangoQuerySet.h>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

DBusDataSource::DBusDataSource(int id, const QString &name,
		QDBusConnection &dbusConnection, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new DataSourceAdaptor(this)), m_id(id), m_path(
				DBusPaths::dataSource(m_id)), m_name(name) {

	// DBus setup
	m_dbusConnection.registerObject(m_path, this);
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
	DataSource::findById(m_id, &dataSource);
	return dataSource.formatString();
}

void DBusDataSource::setFormatString(const QString &formatString) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	if (formatString != dataSource.formatString()) {
		dataSource.setFormatString(formatString);
		if (!dataSource.save()) {
			throw logic_error("couldn't save data source");
		}
		m_adaptor->formatStringChanged(formatString);
	}
}

QString DBusDataSource::textDomain() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.textDomain();
}

void DBusDataSource::setTextDomain(const QString &textDomain) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	if (textDomain != dataSource.textDomain()) {
		dataSource.setTextDomain(textDomain);
		if (!dataSource.save()) {
			throw logic_error("couldn't save data source");
		}
		m_adaptor->textDomainChanged(textDomain);
	}
}
