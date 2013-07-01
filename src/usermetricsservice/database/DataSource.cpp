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

#include <QDjangoQuerySet.h>

using namespace UserMetricsService;

DataSource::DataSource(QObject *parent) :
		QDjangoModel(parent), m_id(0) {
}

DataSource::~DataSource() {
}

int DataSource::id() const {
	return m_id;
}

void DataSource::setId(int id) {
	m_id = id;
}

const QString & DataSource::name() const {
	return m_name;
}

void DataSource::setName(const QString &name) {
	m_name = name;
}

const QString & DataSource::formatString() const {
	return m_formatString;
}

void DataSource::setFormatString(const QString &formatString) {
	m_formatString = formatString;
}

void DataSource::findById(int id, DataSource *dataSource) {
	QDjangoQuerySet<DataSource>().get(
			QDjangoWhere("id", QDjangoWhere::Equals, id), dataSource);
}

void DataSource::findByName(const QString &name, DataSource *dataSource) {
	QDjangoQuerySet<DataSource>().get(
			QDjangoWhere("name", QDjangoWhere::Equals, name), dataSource);
}

bool DataSource::exists(const QString &name) {
	return QDjangoQuerySet<DataSource>().filter(
			QDjangoWhere("name", QDjangoWhere::Equals, name)).limit(0, 1).size()
			> 0;
}
