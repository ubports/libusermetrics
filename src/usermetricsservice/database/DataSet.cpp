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

#include <QDjangoQuerySet.h>

using namespace UserMetricsService;

DataSet::DataSet() :
		m_id(0), m_data() {
}

DataSet::~DataSet() {
}

int DataSet::id() const {
	return m_id;
}

void DataSet::setId(int id) {
	m_id = id;
}

UserData * DataSet::userData() const {
	return qobject_cast<UserData*>(foreignKey("userData"));
}

void DataSet::setUserData(UserData *userData) {
	setForeignKey("userData", userData);
}

DataSource * DataSet::dataSource() const {
	return qobject_cast<DataSource*>(foreignKey("dataSource"));
}

void DataSet::setDataSource(DataSource *dataSource) {
	setForeignKey("dataSource", dataSource);
}

const QByteArray & DataSet::data() const {
	return m_data;
}

void DataSet::setData(const QByteArray &data) {
	m_data = data;
}

const QDate & DataSet::lastUpdated() const {
	return m_lastUpdated;
}

void DataSet::setLastUpdated(const QDate &lastUpdated) {
	m_lastUpdated = lastUpdated;
}

void DataSet::findById(int id, DataSet *dataSet) {
	QDjangoQuerySet<DataSet>().get(QDjangoWhere("id", QDjangoWhere::Equals, id),
			dataSet);
}
