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

#ifndef USERMETRICSINPUT_METRIC_H_
#define USERMETRICSINPUT_METRIC_H_

#include <libusermetricsinput/MetricUpdate.h>
#include <QtCore/QSharedPointer>

/**
 * @{
 */

namespace UserMetricsInput {

class Metric;

/**
 * The instances of Metric are implicitly shared.
 */
typedef QSharedPointer<Metric> MetricPtr;

enum MetricType {
	USER, SYSTEM,
};

/**
 * @brief This class represents a single user metric
 *
 * For example, "number of e-mails today" or "photos taken today".
 *
 * This is a long-lived class that can exist for the whole application
 * lifecycle.
 */
class Q_DECL_EXPORT Metric: public QObject {
public:
	/**
	 * @brief This constructor cannot be used - the class is pure-virtual.
	 */
	explicit Metric(QObject *parent = 0);

	/**
	 * @brief Destructor
	 */
	virtual ~Metric();

	/**
	 * @brief Create an MetricUpdate to a particular Metric
	 *
	 * @param username The user to update the data for. If blank ("")
	 *     or omitted then the current user is used.
	 *
	 * The MetricUpdate object must be deleted - this is when the
	 * actual update will be sent to the storage service.
	 */
	virtual MetricUpdate * update(const QString &username = "") = 0;

	/**
	 * @brief Update the "today" value for a simple user metric
	 *
	 * @param value Today's value
	 * @param username The user to update the data for. If blank ("")
	 *     or omitted then the current user is used.
	 */
	virtual void update(double value, const QString &username = "") = 0;

	/**
	 * @brief Increment the "today" value for a simple user metric
	 *
	 * @param amount How much to increase the metric by - defaults to 1.
	 * @param username The user to update the data for. If blank ("")
	 *     or omitted then the current user is used.
	 */
	virtual void increment(double amount = 1.0f,
			const QString &username = "") = 0;
};

}

/**
 * @}
 **/

#endif // USERMETRICSINPUT_METRIC_H_
