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
 */

#include <MetricInfo.h>

MetricInfo::MetricInfo(QString name, QString format, QString emptyFormat,
		QString domain, QObject *parent) :
		QObject(parent), m_name(name), m_format(format), m_emptyFormat(
				emptyFormat), m_domain(domain) {
}

MetricInfo::~MetricInfo() {
}

QString MetricInfo::name() const {
	return m_name;
}

QString MetricInfo::format() const {
	return m_format;
}

QString MetricInfo::emptyFormat() const {
	return m_emptyFormat;
}

QString MetricInfo::domain() const {
	return m_domain;
}
