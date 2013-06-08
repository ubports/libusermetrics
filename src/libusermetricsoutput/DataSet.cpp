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

#include <libusermetricsoutput/DataSet.h>

using namespace UserMetricsOutput;

DataSet::DataSet(const QString &formatString, const ColorTheme &firstColor,
		const ColorTheme &secondColor, QObject* parent) :
		QObject(parent), m_formatString(formatString), m_firstColor(this), m_secondColor(
				this) {
	m_firstColor.setColors(firstColor);
	m_secondColor.setColors(secondColor);
}

DataSet::~DataSet() {
}

const QString & DataSet::formatString() const {
	return m_formatString;
}

const ColorTheme & DataSet::firstColor() const {
	return m_firstColor;
}

const ColorTheme & DataSet::secondColor() const {
	return m_secondColor;
}

const QVariantList & DataSet::data() const {
	return m_data;
}

const QDate & DataSet::lastUpdated() const {
	return lastUpdated();
}

void DataSet::setData(const QDate &lastUpdated, const QVariantList &data) {
	m_lastUpdated = lastUpdated;
	m_data = data;
	lastUpdatedChanged(m_lastUpdated);
	dataChanged(&m_data);
}
