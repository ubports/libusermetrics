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

/*
 * This file defines the C API of libusermetricsinput. If you can, it is strongly
 * recommended to use the C++ API, MetricManager.h instead.
 */

#ifndef USERMETRICSINPUT_H_
#define USERMETRICSINPUT_H_

#ifdef USERMETRICSINPUT_METRICUPDATE_H_
#error "Mixing C and C++ public header includes. You can only use one or the other."
#endif

#include <libusermetricsinput/UserMetricsInputExport.h>

#ifdef __cplusplus
/**
 * @{
 */
extern "C" {
#endif

/**
 * @brief A central place for registering user metrics.
 */
typedef void* UserMetricsInputMetricManager;

/**
 * @brief A single user metric, e.g. "number of e-mails today" or "photos taken today".
 */
typedef void* UserMetricsInputMetric;

/**
 * @brief A update to a user metric for a particular user.
 */
typedef void* UserMetricsInputMetricUpdate;


/**
 * @brief Construct a new UserMetricsInputMetricManager
 *
 * Must be freed with #usermetricsinput_metricmanager_delete
 */
USERMETRICSINPUT_EXPORT
UserMetricsInputMetricManager usermetricsinput_metricmanager_new();

/**
 * @brief Free a UserMetricsInputMetricManager
 *
 * @param metricManager The UserMetricsInputMetricManager to free
 */
USERMETRICSINPUT_EXPORT
void usermetricsinput_metricmanager_delete(
		UserMetricsInputMetricManager metricManager);

/**
 * @brief Register a new UserMetricsInputMetric
 *
 * @param metricManager
 * @param dataSourceId The unique ID of the data source, e.g. "facebook"
 * @param formatString The string to print in the output API, e.g. "<b>%1</b> messages received today"
 * @param emptyDataString The string to print in the case of no data, e.g. "No messages received today"
 * @param textDomain The translation domain
 *
 * Do not attempt to delete this object.
 */
USERMETRICSINPUT_EXPORT
UserMetricsInputMetric usermetricsinput_metricmanager_add(
		UserMetricsInputMetricManager metricManager, const char *dataSourceId,
		const char *formatString, const char *emptyDataString,
		const char *textDomain);

/**
 * @brief Increment the "today" value for a simple user metric
 *
 * @param metric
 * @param amount How much to increase the metric by - usually 1.
 * @param username The user to update the data for. If blank ("") then the current user is used.
 */
USERMETRICSINPUT_EXPORT
void usermetricsinput_metric_increment(UserMetricsInputMetric metric,
		double amount, const char *username);

/**
 * @brief Update the "today" value for a simple user metric
 *
 * @param metric
 * @param value Today's value
 * @param username The user to update the data for. If blank ("") then the current user is used.
 */
USERMETRICSINPUT_EXPORT
void usermetricsinput_metric_update_today(UserMetricsInputMetric metric,
		double value, const char *username);

/**
 * @brief Create an update to a particular metric
 *
 * @param metric
 * @param username The user to update the data for. If blank ("") then the current user is used.
 *
 * Must be freed with #usermetricsinput_metricupdate_delete - this is when the
 * actual update will be sent to the storage service.
 */
USERMETRICSINPUT_EXPORT
UserMetricsInputMetricUpdate usermetricsinput_metric_update(
		UserMetricsInputMetric metric, const char *username);

/**
 * @brief Free a UserMetricsInputMetric
 *
 * @param metricUpdate The UserMetricsInputMetric to free and dispatch
 *
 * This will cause the update to be dispatched.
 */
USERMETRICSINPUT_EXPORT
void usermetricsinput_metricupdate_delete(
		UserMetricsInputMetricUpdate metricUpdate);

/**
 * @brief Add data to a UserMetricsInputMetricUpdate.
 *
 * @param metricUpdate The UserMetricsInputMetricUpdate to add data to
 * @param data The double-valued data to add
 *
 * Each call to this method appends a new day's data to the update.
 * So the first call adds data for today, the second call adds data for
 * yesterday, and so on.
 */
USERMETRICSINPUT_EXPORT
void usermetricsinput_metricupdate_add_data(
		UserMetricsInputMetricUpdate metricUpdate, double data);

/**
 * @brief Add unknown data to an update.
 *
 * @param metricUpdate The UserMetricsInputMetricUpdate to add data to
 *
 * If data is not known for a particular day, then null can be added.
 */
USERMETRICSINPUT_EXPORT
void usermetricsinput_metricupdate_add_null(
		UserMetricsInputMetricUpdate metricUpdate);

/**
 * @example MetricManagerTodayCAPI.c
 *   A simple example of a metric that doesn't worry about updating
 *   history, and just increments a single value in response to an
 *   action.
 **/

/**
 * @example MetricManagerAdvancedCAPI.c
 *   A more advanced metric that updates the history of a metric.
 *   Usually this would be a metric from an external data source,
 *   e.g. e-mail or Facebook messages.
 **/

#ifdef __cplusplus
}
/**
 * @}
 **/
#endif

#endif
