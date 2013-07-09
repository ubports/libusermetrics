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

#ifndef USERMETRICSINPUT_METRICUPDATE_H_
#define USERMETRICSINPUT_METRICUPDATE_H_

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

/**
 * @{
 */

namespace UserMetricsInput {

class MetricUpdate;

/**
 * @brief Useful to store your instance of MetricUpdate in.
 **/
typedef QScopedPointer<MetricUpdate> MetricUpdatePtr;

/**
 * @brief An update to a Metric
 *
 * This is a short-lived class representing an update for a particular
 * use to a Metric.
 **/
class Q_DECL_EXPORT MetricUpdate: public QObject {
public:
	/**
	 * @brief This constructor cannot be used - the class is pure-virtual.
	 */
	explicit MetricUpdate(QObject *parent = 0);

	/**
	 * @brief Destructor
	 */
	virtual ~MetricUpdate();

	/**
	 * @brief Add data to a UserMetricsInputMetricUpdate.
	 *
	 * @param data The double-valued data to add
	 *
	 * Each call to this method appends a new day's data to the update.
	 * So the first call adds data for today, the second call adds data for
	 * yesterday, and so on.
	 */
	virtual void addData(double data) = 0;

	/**
	 * @brief Add unknown data to an update.
	 *
	 * If data is not known for a particular day, then null can be added.
	 */
	virtual void addNull() = 0;
};

}

/**
 * @}
 **/

#endif // USERMETRICSINPUT_METRICUPDATE_H_
