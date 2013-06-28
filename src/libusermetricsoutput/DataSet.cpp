/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <libusermetricsoutput/DataSet.h>

using namespace UserMetricsOutput;

DataSet::DataSet(QObject* parent) :
		QObject(parent) {
}

DataSet::~DataSet() {
}

const QString & DataSet::formatString() const {
	return m_formatString;
}

const QVariantList & DataSet::data() const {
	return m_data;
}

const QDate & DataSet::lastUpdated() const {
	return m_lastUpdated;
}

void DataSet::setFormatString(const QString &formatString) {
	if (formatString != m_formatString) {
		m_formatString = formatString;
		formatStringChanged(m_formatString);
	}
}

void DataSet::update(const QDate &lastUpdated, const QVariantList &data) {
	m_lastUpdated = lastUpdated;
	m_data = data;
	lastUpdatedChanged(m_lastUpdated);
	dataChanged(&m_data);
}
