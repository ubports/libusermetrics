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

#ifndef USERMETRICSINPUT_METRICMANAGER_H_
#define USERMETRICSINPUT_METRICMANAGER_H_

#include <libusermetricsinput/Metric.h>

/**
 * @{
 */

/**
 * @brief The user metrics input library namespace
 **/
namespace UserMetricsInput {

class MetricManager;

/**
 * @brief Shared pointer for the MetricManager
 **/
typedef QScopedPointer<MetricManager> MetricManagerPtr;

/**
 * @brief Central place for registering and updating user metrics.
 *
 * This is a long-lived class that can exist for the whole application
 * lifecycle.
 **/
class Q_DECL_EXPORT MetricManager: public QObject {
public:
	/**
	 * @brief You cannot use this constructor. This is a pure-virtual class.
	 */
	explicit MetricManager(QObject *parent = 0);

	/**
	 * @brief Destructs the metric manager and associated Metric instances.
	 */
	virtual ~MetricManager();

	/**
	 * @brief Gets a new instance of the MetricManager.
	 *
	 * If you want a singleton instance then hold onto this.
	 */
	static MetricManager * getInstance();

	/**
	 * @brief Register a new Metric.
	 *
	 * @param dataSourceId The unique ID of the data source, e.g. "facebook"
	 * @param formatString The string to print in the output API, e.g. "<b>%1</b> messages received today"
	 * @param emptyDataString The string to print in the case of no data, e.g. "No messages received today"
	 * @param textDomain The translation domain
	 *
	 * This will register a new user Metric with the above paramters. It is
	 * acceptable to call this method more than once. The same Metric instance
	 * will be returned.
	 */
	virtual MetricPtr add(const QString &dataSourceId,
			const QString &formatString, const QString &emptyDataString = "",
			const QString &textDomain = "") = 0;
};

/**
 * @example MetricManagerToday.cpp
 *   A simple example of a metric that doesn't worry about updating
 *   history, and just increments a single value in response to an
 *   action.
 **/

/**
 * @example MetricManagerAdvanced.cpp
 *   A more advanced metric that updates the history of a metric.
 *   Usually this would be a metric from an external data source,
 *   e.g. e-mail or Facebook messages.
 **/
}

/**
 * @}
 **/

#endif // USERMETRICSINPUT_METRICMANAGER_H_
