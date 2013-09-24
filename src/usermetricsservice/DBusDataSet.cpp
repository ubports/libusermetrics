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

#include <usermetricsservice/database/DataSet.h>
#include <usermetricsservice/Authentication.h>
#include <usermetricsservice/DBusDataSet.h>
#include <usermetricsservice/DataSetAdaptor.h>
#include <libusermetricscommon/DateFactory.h>
#include <libusermetricscommon/DBusPaths.h>
#include <libusermetricscommon/Localisation.h>

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

#include <QDjangoQuerySet.h>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

DBusDataSet::DBusDataSet(int id, const QString &dataSource,
		QDBusConnection &dbusConnection,
		QSharedPointer<DateFactory> dateFactory,
		QSharedPointer<Authentication> authentication, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new DataSetAdaptor(this)), m_dateFactory(dateFactory), m_authentication(
				authentication), m_id(id), m_path(DBusPaths::dataSet(m_id)), m_dataSource(
				dataSource) {

	// DBus setup
	m_dbusConnection.registerObject(m_path, this);
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

void DBusDataSet::internalUpdate(DataSet &dataSet, const QVariantList &oldData,
		const QVariantList &data) {
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
				newData.append(QVariant(""));
			}
			// append the whole of the old data
			newData.append(oldData);
		}
	}

	while (newData.size() > 62) {
		newData.pop_back();
	}

	QByteArray byteArray;
	{
		QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
		dataStream << newData;
	}

	dataSet.setLastUpdated(currentDate);
	dataSet.setData(byteArray);
	if (!dataSet.save()) {
		throw logic_error(_("Could not save data set"));
	}

	QDateTime dateTime(currentDate);
	m_adaptor->updated(dateTime.toTime_t(), newData);
}

void DBusDataSet::update(const QVariantList &data) {
	DataSet dataSet;
	DataSet::findByIdRelated(m_id, &dataSet);

	QString dbusUsername(
			m_authentication->getUsername(m_dbusConnection, *this));
	const QString &username(dataSet.userData()->username());
	if (!dbusUsername.isEmpty() && !username.isEmpty()
			&& dbusUsername != username) {
		//FIXME Signal error about data owner
		return;
	}

	QString confinementContext(
			m_authentication->getConfinementContext(m_dbusConnection, *this));
	const QString &secret(dataSet.dataSource()->secret());
	if (secret != "unconfined" && secret != confinementContext) {
		//FIXME Signal error about data source owner
		return;
	}

	QVariantList oldData;
	getData(dataSet, oldData);

	internalUpdate(dataSet, oldData, data);
}

void DBusDataSet::increment(double amount) {
	DataSet dataSet;
	DataSet::findByIdRelated(m_id, &dataSet);

	QString dbusUsername(
			m_authentication->getUsername(m_dbusConnection, *this));
	const QString &username(dataSet.userData()->username());
	if (!dbusUsername.isEmpty() && !username.isEmpty()
			&& dbusUsername != username) {
		//FIXME Signal error about data owner
		return;
	}

	QString confinementContext(
			m_authentication->getConfinementContext(m_dbusConnection, *this));
	const QString &secret(dataSet.dataSource()->secret());
	if (secret != "unconfined" && secret != confinementContext) {
		//FIXME Signal error about data source owner
		return;
	}

	QVariantList oldData;
	getData(dataSet, oldData);

	QVariantList data;

	QDate currentDate(m_dateFactory->currentDate());
	if (dataSet.lastUpdated() == currentDate && !oldData.empty()) {
		double value(oldData.first().toDouble());
		value += amount;
		data << value;
	} else {
		data << amount;
	}

	internalUpdate(dataSet, oldData, data);
}

uint DBusDataSet::lastUpdated() const {
	const QDate &lastUpdated(lastUpdatedDate());
	QDateTime dateTime(lastUpdated);
	return dateTime.toTime_t();
}

QDate DBusDataSet::lastUpdatedDate() const {
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

QString DBusDataSet::dataSource() const {
	return m_dataSource;
}
