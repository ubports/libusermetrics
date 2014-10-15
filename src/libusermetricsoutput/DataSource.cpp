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

DataSource::DataSource(const QString &localeDir, QObject *parent) :
		QObject(parent), m_formatString(""), m_formatStringTr(""), m_emptyDataString(
				""), m_emptyDataStringTr(""), m_textDomain(""), m_localeDir(
				localeDir), m_type(USER), m_externalGettext(ExternalGettext::singletonInstance()) {
}

DataSource::~DataSource() {
}

const QString & DataSource::formatString() const {
	return m_formatStringTr;
}

void DataSource::translationResult(const QString &result, const QString &requestId)
{
	if (requestId == "format-string") {
		if (result != m_formatStringTr) {
			m_formatStringTr = result;
			formatStringChanged (m_formatStringTr);
		}
	} else if (requestId == "empty-data-string") {
		if (result != m_emptyDataStringTr) {
			m_emptyDataStringTr = result;
			emptyDataStringChanged(m_emptyDataStringTr);
		}
	}
}

void DataSource::updateFormatStringTranslation() {
	m_externalGettext->tr(m_textDomain, m_formatString,
			m_localeDir, this, "format-string");
}

void DataSource::setFormatString(const QString &formatString) {
	if (formatString != m_formatString) {
		m_formatString = formatString;
		updateFormatStringTranslation();
	}
}

void DataSource::updateEmptyDataStringTranslation() {
	m_externalGettext->tr(m_textDomain, m_emptyDataString,
				m_localeDir, this, "empty-data-string");
}

const QString & DataSource::emptyDataString() const {
	return m_emptyDataStringTr;
}

void DataSource::setEmptyDataString(const QString &emptyDataString) {
	if (emptyDataString != m_emptyDataString) {
		m_emptyDataString = emptyDataString;
		updateEmptyDataStringTranslation();
	}
}

const QString & DataSource::textDomain() const {
	return m_textDomain;
}

void DataSource::setTextDomain(const QString &textDomain) {
	if (textDomain != m_textDomain) {
		m_textDomain = textDomain;
		updateFormatStringTranslation();
		updateEmptyDataStringTranslation();
		textDomainChanged(m_textDomain);
	}
}

MetricType DataSource::type() const {
	return m_type;
}

void DataSource::setType(const MetricType type) {
	if (type != m_type) {
		m_type = type;
		typeChanged(m_type);
	}
}

const QVariantMap & DataSource::options() const {
	return m_options;
}

void DataSource::setOptions(const QVariantMap &options) {
	if (options != m_options) {
		m_options = options;
		optionsChanged(m_options);
	}
}
