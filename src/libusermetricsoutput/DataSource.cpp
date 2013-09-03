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
#include <libusermetricscommon/Localisation.h>

using namespace UserMetricsOutput;

DataSource::DataSource(const QString &localeDir, QObject *parent) :
		QObject(parent), m_formatString(""), m_formatStringTr(""), m_emptyDataString(
				""), m_emptyDataStringTr(""), m_textDomain(""), m_localeDir(
				localeDir) {

}

DataSource::~DataSource() {
}

const QString & DataSource::formatString() const {
	return m_formatStringTr;
}

void DataSource::updateFormatStringTranslation() {
	QString newFormatStringTr(
			gettextExternal(m_textDomain, m_formatString, m_localeDir));
	if (newFormatStringTr != m_formatStringTr) {
		m_formatStringTr = newFormatStringTr;
		formatStringChanged(m_formatStringTr);
	}
}

void DataSource::setFormatString(const QString &formatString) {
	if (formatString != m_formatString) {
		m_formatString = formatString;
		updateFormatStringTranslation();
	}
}

void DataSource::updateEmptyDataStringTranslation() {
	QString newEmptyDataStringTr(
			gettextExternal(m_textDomain, m_emptyDataString, m_localeDir));
	if (newEmptyDataStringTr != m_emptyDataStringTr) {
		m_emptyDataStringTr = newEmptyDataStringTr;
		emptyDataStringChanged(m_emptyDataStringTr);
	}
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

const QVariantMap & DataSource::options() const {
	return m_options;
}

void DataSource::setOptions(const QVariantMap &options) {
	if (options != m_options) {
		m_options = options;
		optionsChanged(m_options);
	}
}
