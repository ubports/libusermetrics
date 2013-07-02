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

#include <libusermetricsinput/usermetricsinput.h>
#include <libusermetricsinput/MetricManager.h>

#include <stdexcept>
#include <cstdio>

using namespace UserMetricsInput;
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

UserMetricsInputMetricManager usermetricsinput_metricmanager_new() {
	try {
		MetricManagerPtr metricManager(MetricManager::getInstance());
		return reinterpret_cast<UserMetricsInputMetricManager>(metricManager.take());
	} catch (exception &e) {
		fprintf(stderr, "Error creating MetricManager: %s\n", e.what());
	}
	return nullptr;
}

void usermetricsinput_metricmanager_delete(
		UserMetricsInputMetricManager metricManager) {
	try {
		delete reinterpret_cast<MetricManager*>(metricManager);
	} catch (exception &e) {
		fprintf(stderr, "Error deleting Word: %s\n", e.what());
	}
}

UserMetricsInputMetric usermetricsinput_metricmanager_add(
		UserMetricsInputMetricManager m, const char *dataSourceId,
		const char *formatString) {
	try {
		MetricManager *metricManager(reinterpret_cast<MetricManager*>(m));
		MetricPtr metric(metricManager->add(dataSourceId, formatString));
		return reinterpret_cast<UserMetricsInputMetric>(metric.data());
	} catch (exception &e) {
		fprintf(stderr, "Error adding metric: %s\n", e.what());
	}
	return nullptr;
}

UserMetricsInputMetricUpdate usermetricsinput_metric_update(
		UserMetricsInputMetric m, const char *username) {
	try {
		Metric *metric(reinterpret_cast<Metric*>(m));
		MetricUpdatePtr metricUpdate(
				metric->update(QString::fromUtf8(username)));
		return reinterpret_cast<UserMetricsInputMetric>(metricUpdate.take());
	} catch (exception &e) {
		fprintf(stderr, "Error adding metric: %s\n", e.what());
	}
	return nullptr;
}

void usermetricsinput_metricupdate_delete(
		UserMetricsInputMetricUpdate metricUpdate) {
	try {
		delete reinterpret_cast<MetricUpdate*>(metricUpdate);
	} catch (exception &e) {
		fprintf(stderr, "Error deleting MetricUpdate: %s\n", e.what());
	}
}

void usermetricsinput_metricupdate_add_data(UserMetricsInputMetricUpdate u,
		float data) {
	try {
		MetricUpdate *metricUpdate = reinterpret_cast<MetricUpdate*>(u);
		metricUpdate->addData(data);
	} catch (exception &e) {
		fprintf(stderr, "Error adding data: %s\n", e.what());
	}
}

void usermetricsinput_metricupdate_add_null(UserMetricsInputMetricUpdate u) {
	try {
		MetricUpdate *metricUpdate = reinterpret_cast<MetricUpdate*>(u);
		metricUpdate->addNull();
	} catch (exception &e) {
		fprintf(stderr, "Error adding null: %s\n", e.what());
	}
}

#ifdef __cplusplus
}
#endif
