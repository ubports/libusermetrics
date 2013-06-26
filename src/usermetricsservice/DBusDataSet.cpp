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

void DBusDataSet::getData(DataSet &dataSet, QVariantList &data) {
	QDataStream dataStream(dataSet.data());
	dataStream >> data;
}

QVariantList DBusDataSet::data() const {
	DataSet dataSet;
	DataSet::findById(m_id, &dataSet);

	QVariantList data;
	getData(dataSet, data);

	return data;
}

void DBusDataSet::update(const QVariantList &data) {
	DataSet dataSet;
	DataSet::findById(m_id, &dataSet);

	QVariantList oldData;
	getData(dataSet, oldData);

	QDate currentDate(m_dateFactory->currentDate());
	int daysSinceLastUpdate(dataSet.lastUpdated().daysTo(currentDate));

	QVariantList newData(data);

	// if we are in this situation we do nothing
	// new: |4|5|6|7|8|9|0|
	// old:     |1|2|3|
	// res: |4|5|6|7|8|9|0|
	if (daysSinceLastUpdate + oldData.size() > newData.size()) {
		if (daysSinceLastUpdate < newData.size()) {
			// if we are in this situation - we need the
			// protruding data from old
			// new: |6|7|8|9|0|
			// old:     |1|2|3|4|5|
			// res: |6|7|8|9|0|4|5|
			auto oldDataIt(oldData.constBegin());
			// wind forward until the data we want
			for (int i(daysSinceLastUpdate); i < newData.size(); ++i) {
				++oldDataIt;
			}
			// append the rest of the data
			for (; oldDataIt != oldData.constEnd(); ++oldDataIt) {
				newData.append(*oldDataIt);
			}
		} else {
			// we are in this situation - there is a gap
			// and we want the whole of the old data appending
			// new: |6|7|8|9|0|
			// old:             |1|2|3|4|5|
			// res: |6|7|8|9|0|n|1|2|3|4|5|
			const int daysToPad(daysSinceLastUpdate - newData.size());
			// pad the data will null variants
			for (int i(0); i < daysToPad; ++i) {
				newData.append(QVariant());
			}
			// append the whole of the old data
			newData.append(oldData);
		}
	}

	QByteArray byteArray;
	{
		QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
		dataStream << newData;
	}

	dataSet.setLastUpdated(currentDate);
	dataSet.setData(byteArray);
	Q_ASSERT(dataSet.save());
}

QDate DBusDataSet::lastUpdated() const {
	DataSet dataSet;
	DataSet::findById(m_id, &dataSet);
	return dataSet.lastUpdated();
}

int DBusDataSet::id() const {
	return m_id;
}

QString DBusDataSet::path() const {
	return m_path;
}
