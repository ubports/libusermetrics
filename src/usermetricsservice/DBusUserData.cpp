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

#include <usermetricsservice/DBusUserData.h>
#include <usermetricsservice/DBusDataSet.h>
#include <usermetricsservice/UserDataAdaptor.h>
#include <usermetricsservice/database/DataSet.h>
#include <usermetricsservice/database/DataSource.h>
#include <usermetricsservice/database/UserData.h>
#include <libusermetricscommon/DateFactory.h>
#include <libusermetricscommon/DBusPaths.h>
#include <libusermetricscommon/Localisation.h>

#include <QDjangoQuerySet.h>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

DBusUserData::DBusUserData(int id, const QString &username,
		DBusUserMetrics &userMetrics, QDBusConnection &dbusConnection,
		QSharedPointer<DateFactory> dateFactory, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new UserDataAdaptor(this)), m_dateFactory(dateFactory), m_userMetrics(
				userMetrics), m_id(id), m_path(DBusPaths::userData(m_id)), m_username(
				username) {

	// DBus setup
	if (!m_dbusConnection.registerObject(m_path, this)) {
		throw logic_error(_("Could not register user data object with DBus"));
	}

// Database setup
	syncDatabase();
}

DBusUserData::~DBusUserData() {
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.unregisterObject(m_path);
}

QString DBusUserData::path() const {
	return m_path;
}

QString DBusUserData::username() const {
	return m_username;
}

QList<QDBusObjectPath> DBusUserData::dataSets() const {
	QList<QDBusObjectPath> dataSets;
	for (DBusDataSetPtr dataSet : m_dataSets.values()) {
		dataSets << QDBusObjectPath(dataSet->path());
	}
	return dataSets;
}

QDBusObjectPath DBusUserData::createDataSet(const QString &dataSourceName) {
	if (!DataSource::exists(dataSourceName)) {
		qWarning() << _("Unknown data source") << ": [" << dataSourceName
				<< "]";
		return QDBusObjectPath();
	}

	QDjangoQuerySet<DataSet> dataSets;
	QDjangoQuerySet<DataSet> query = dataSets.filter(
			QDjangoWhere("userData_id", QDjangoWhere::Equals, m_id)).filter(
			QDjangoWhere("dataSource__name", QDjangoWhere::Equals,
					dataSourceName));

	if (query.size() == -1) {
		throw logic_error(_("Data set query failed"));
	}

	DataSet dataSet;

	if (query.size() == 0) {
		UserData userData;
		UserData::findById(m_id, &userData);
		dataSet.setUserData(&userData);

		DataSource dataSource;
		DataSource::findByName(dataSourceName, &dataSource);
		dataSet.setDataSource(&dataSource);

		if (!dataSet.save()) {
			throw logic_error(_("Could not save data set"));
		}

		syncDatabase();
	} else {
		query.at(0, &dataSet);
	}

	DBusDataSetPtr dataSetPtr(m_dataSets.value(dataSet.id()));
	if (dataSetPtr.isNull()) {
		throw logic_error(_("New data set could not be found"));
	}
	return QDBusObjectPath(dataSetPtr->path());
}

void DBusUserData::syncDatabase() {
	QSet<int> dataSetIds;
	QDjangoQuerySet<DataSet> dataSets;
	QDjangoQuerySet<DataSet> query(
			dataSets.filter(
					QDjangoWhere("userData_id", QDjangoWhere::Equals, m_id)));
	for (const DataSet &dataSet : query.selectRelated()) {
		const int id(dataSet.id());
		dataSetIds << id;
		// if we don't have a local cache
		if (!m_dataSets.contains(id)) {
			DBusDataSetPtr dbusDataSet(
					new DBusDataSet(id, dataSet.dataSource()->name(),
							m_dbusConnection, m_dateFactory));
			m_dataSets.insert(id, dbusDataSet);
			m_adaptor->dataSetAdded(dbusDataSet->dataSource(),
					QDBusObjectPath(dbusDataSet->path()));
		}
	}
	// remove any cached references to deleted data sets
	QSet<int> cachedDataSetIds(QSet<int>::fromList(m_dataSets.keys()));
	QSet<int> &toRemove(cachedDataSetIds.subtract(dataSetIds));
	for (int id : toRemove) {
		DBusDataSetPtr dataSet = m_dataSets.take(id);
		m_adaptor->dataSetRemoved(dataSet->dataSource(),
				QDBusObjectPath(dataSet->path()));
	}
}

DBusDataSetPtr DBusUserData::dataSet(const QString &dataSource) const {
	QDjangoQuerySet<DataSet> dataSets;
	QScopedPointer<DataSet> dataSet(
			dataSets.filter(
					QDjangoWhere("userData_id", QDjangoWhere::Equals, m_id)).get(
					QDjangoWhere("dataSource__name", QDjangoWhere::Equals,
							dataSource)));

	return m_dataSets.value(dataSet->id());
}
