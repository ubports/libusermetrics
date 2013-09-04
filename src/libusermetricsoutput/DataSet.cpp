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

using namespace std;
using namespace UserMetricsOutput;

DataSet::DataSet(DataSourcePtr dataSource, QObject* parent) :
		QObject(parent), m_dataSource(dataSource) {

	connect(m_dataSource.data(), SIGNAL(optionsChanged(const QVariantMap &)),
			this, SLOT(optionsChanged(const QVariantMap &)));
}

DataSet::~DataSet() {
}

const QVariantList & DataSet::data() const {
	return m_data;
}

const QDate & DataSet::lastUpdated() const {
	return m_lastUpdated;
}

const QVariant & DataSet::head() const {
	return m_head;
}

void DataSet::setData(const QVariantList &data) {
	m_originalData = data;
	scaleData();
}

void DataSet::scaleData() {
	m_data = m_originalData;

	const QVariantMap &options(m_dataSource->options());

	bool hasMinimum(options.contains("minimum"));
	bool hasMaximum(options.contains("maximum"));

	double min(numeric_limits<double>::max());
	double max(numeric_limits<double>::min());

	if (hasMinimum) {
		min = options["minimum"].toDouble();
	}
	if (hasMaximum) {
		max = options["maximum"].toDouble();
	}

	// if we need to find either the max or the min
	if (!hasMinimum || !hasMaximum) {
		for (QVariant &variant : m_data) {
			if (variant.type() == QVariant::String) {
				variant = QVariant();
			} else if (variant.type() == QVariant::Double) {
				double value(variant.toDouble());
				if (!hasMinimum && value < min) {
					min = value;
				}
				if (!hasMaximum && value > max) {
					max = value;
				}
			}
		}
	}

	QVariant head;
	if (m_data.isEmpty()) {
		head = QVariant();
	} else {
		head = m_data.first();
	}
	if (m_head != head) {
		m_head = head;
		headChanged(m_head);
	}

	for (QVariant &variant : m_data) {
		if (variant.type() == QVariant::Double) {
			if (min != max) {
				double value(variant.toDouble());
				if (hasMaximum && value > max) {
					value = max;
				}
				if (hasMinimum && value < min) {
					value = min;
				}
				variant = (value - min) / (max - min);
			} else {
				variant = 0.5;
			}
		}
	}

	dataChanged(&m_data);
}

void DataSet::setLastUpdated(const QDate &lastUpdated) {
	if (m_lastUpdated != lastUpdated) {
		m_lastUpdated = lastUpdated;
		lastUpdatedChanged(m_lastUpdated);
	}
}

void DataSet::update(const uint lastUpdated, const QVariantList &data) {
	setLastUpdated(QDateTime::fromTime_t(lastUpdated).date());
	setData(data);
}

void DataSet::optionsChanged(const QVariantMap &options) {
	Q_UNUSED(options);
	scaleData();
}
