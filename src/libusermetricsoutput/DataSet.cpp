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

int DataSet::calculateLength() {
	int day(m_firstMonth.size());
	auto it = m_firstMonth.end(), end = m_firstMonth.begin();
	while (it != end) {
		--it;
		--day;
		if (!it->isNull()) {
			return day;
		}
	}

	return -1;
}

DataSet::DataSet(QObject *parent) :
		QObject(parent), m_firstColor(this), m_secondColor(this) {
	m_length = calculateLength();
}

DataSet::DataSet(const QString &label, const ColorTheme &firstColor,
		const QVariantList &firstMonth, const ColorTheme &secondColor,
		const QVariantList &secondMonth, QObject* parent) :
		QObject(parent), m_label(label), m_firstColor(this), m_firstMonth(
				firstMonth), m_secondColor(this), m_secondMonth(secondMonth) {
	m_length = calculateLength();
	m_firstColor.setColors(firstColor);
	m_secondColor.setColors(secondColor);
}

DataSet::~DataSet() {
}

const QString & DataSet::label() const {
	return m_label;
}

const ColorTheme & DataSet::firstColor() const {
	return m_firstColor;
}

const QVariantList & DataSet::firstMonth() const {
	return m_firstMonth;
}

const ColorTheme & DataSet::secondColor() const {
	return m_secondColor;
}

const QVariantList & DataSet::secondMonth() const {
	return m_secondMonth;
}

int DataSet::length() const {
	return m_length;
}
