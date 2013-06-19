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
extern "C" {
#endif

typedef void* UserMetricsInputMetric;
typedef void* UserMetricsInputMetricManager;
typedef void* UserMetricsInputMetricUpdate;

USERMETRICSINPUT_EXPORT UserMetricsInputMetricManager usermetricsinput_metricmanager_new();
USERMETRICSINPUT_EXPORT void usermetricsinput_metricmanager_delete(
		UserMetricsInputMetricManager metricManager);
USERMETRICSINPUT_EXPORT UserMetricsInputMetric usermetricsinput_metricmanager_add(
		UserMetricsInputMetricManager metricManager, const char *dataSourceId,
		const char *formatString);

USERMETRICSINPUT_EXPORT UserMetricsInputMetricUpdate usermetricsinput_metric_update(
		UserMetricsInputMetric metric, const char *username);

USERMETRICSINPUT_EXPORT void usermetricsinput_metricupdate_delete(
		UserMetricsInputMetricUpdate metricUpdate);
USERMETRICSINPUT_EXPORT void usermetricsinput_metricupdate_add_data(
		UserMetricsInputMetricUpdate metricUpdate, float data);
USERMETRICSINPUT_EXPORT void usermetricsinput_metricupdate_add_null(
		UserMetricsInputMetricUpdate metricUpdate);

#ifdef __cplusplus
}
#endif

#endif
