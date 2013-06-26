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

#include <usermetricsservice/DBusDataSet.h>
#include <usermetricsservice/DataSetAdaptor.h>

using namespace UserMetricsService;

DBusDataSet::DBusDataSet(int id, QDBusConnection &dbusConnection,
		QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new DataSetAdaptor(this)), m_id(id), m_path(
				QString("/com/canonical/UserMetrics/DataSet/%1").arg(m_id)) {

	// DBus setup
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.registerObject(m_path, this);
}

DBusDataSet::~DBusDataSet() {
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.unregisterObject(m_path);
}

void DBusDataSet::update(const QVariantList &data) {

}

int DBusDataSet::id() const {
	return m_id;
}

QString DBusDataSet::path() const {
	return m_path;
}
