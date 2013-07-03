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

#include <libusermetricsoutput/DataSource.h>

using namespace UserMetricsOutput;

DataSource::DataSource(QObject *parent) :
		QObject(parent), m_formatString("") {

}

DataSource::~DataSource() {
}

const QString & DataSource::formatString() const {
	return m_formatString;
}

void DataSource::setFormatString(const QString &formatString) {
	if (formatString != m_formatString) {
		m_formatString = formatString;
		formatStringChanged(m_formatString);
	}
}

const QString & DataSource::emptyDataString() const {
	return m_emptyDataString;
}

void DataSource::setEmptyDataString(const QString &emptyDataString) {
	if (emptyDataString != m_emptyDataString) {
		m_emptyDataString = emptyDataString;
		emptyDataStringChanged (m_emptyDataString);
	}
}

const QString & DataSource::textDomain() const {
	return m_textDomain;
}

void DataSource::setTextDomain(const QString &textDomain) {
	if (textDomain != m_textDomain) {
		m_textDomain = textDomain;
		textDomainChanged(m_textDomain);
	}
}
