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
		QDjangoModel(parent), m_id(0), m_secret(), m_type(0), m_hasMinimum(
				false), m_minimum(0), m_hasMaximum(false), m_maximum(0) {
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

const QString & DataSource::secret() const {
	return m_secret;
}

void DataSource::setSecret(const QString &secret) {
	m_secret = secret;
}

const QString & DataSource::formatString() const {
	return m_formatString;
}

void DataSource::setFormatString(const QString &formatString) {
	m_formatString = formatString;
}

const QString & DataSource::emptyDataString() const {
	return m_emptyDataString;
}

void DataSource::setEmptyDataString(const QString &emptyDataString) {
	m_emptyDataString = emptyDataString;
}

const QString & DataSource::textDomain() const {
	return m_textDomain;
}

void DataSource::setTextDomain(const QString &textDomain) {
	m_textDomain = textDomain;
}

int DataSource::type() const {
	return m_type;
}

void DataSource::setType(int type) {
	m_type = type;
}

bool DataSource::hasMinimum() const {
	return m_hasMinimum;
}

void DataSource::setHasMinimum(bool hasMinimum) {
	m_hasMinimum = hasMinimum;
}

double DataSource::minimum() const {
	return m_minimum;
}

void DataSource::setMinimum(double minimum) {
	m_minimum = minimum;
}

bool DataSource::hasMaximum() const {
	return m_hasMaximum;
}

void DataSource::setHasMaximum(bool hasMaximum) {
	m_hasMaximum = hasMaximum;
}

double DataSource::maximum() const {
	return m_maximum;
}

void DataSource::setMaximum(double maximum) {
	m_maximum = maximum;
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
