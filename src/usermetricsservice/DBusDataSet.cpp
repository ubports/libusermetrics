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

#include <usermetricsservice/database/DataSet.h>
#include <usermetricsservice/DBusDataSet.h>
#include <usermetricsservice/DataSetAdaptor.h>
#include <libusermetricscommon/DateFactory.h>

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

using namespace UserMetricsCommon;
using namespace UserMetricsService;

DBusDataSet::DBusDataSet(int id, QDBusConnection &dbusConnection,
		QSharedPointer<DateFactory> dateFactory, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new DataSetAdaptor(this)), m_dateFactory(dateFactory), m_id(id), m_path(
				QString("/com/canonical/UserMetrics/DataSet/%1").arg(m_id)) {

	// DBus setup
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.registerObject(m_path, this);
}

DBusDataSet::~DBusDataSet() {
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.unregisterObject(m_path);
}

QVariantList DBusDataSet::data() const {
	DataSet dataSet;
	DataSet::findById(m_id, &dataSet);

	const QByteArray &byteArray(dataSet.data());
	QVariantList data;
	{
		QDataStream dataStream(byteArray);
		dataStream >> data;
	}

	return data;
}

void DBusDataSet::update(const QVariantList &data) {
	QByteArray byteArray;
	{
		QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
		dataStream << data;
	}

	DataSet dataSet;
	DataSet::findById(m_id, &dataSet);

	dataSet.setLastUpdated(m_dateFactory->currentDate());
	dataSet.setData(byteArray);
	Q_ASSERT(dataSet.save());
}

int DBusDataSet::id() const {
	return m_id;
}

QString DBusDataSet::path() const {
	return m_path;
}
