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
		fprintf(stderr, "Error deleting MetricManager: %s\n", e.what());
	}
}

UserMetricsInputMetricParameters usermetricsinput_metricparameters_new(
		const char *dataSourceId) {
	try {
		return reinterpret_cast<UserMetricsInputMetricParameters>(new MetricParameters(
				dataSourceId));
	} catch (exception &e) {
		fprintf(stderr, "Error creating MetricParameters: %s\n", e.what());
	}
	return nullptr;
}

void usermetricsinput_metricparameters_set_format_string(
		UserMetricsInputMetricParameters p, const char *formatString) {
	try {
		MetricParameters *metricParameters(
				reinterpret_cast<MetricParameters*>(p));
		metricParameters->formatString(formatString);
	} catch (exception &e) {
		fprintf(stderr, "Error setting format string: %s\n", e.what());
	}
}

void usermetricsinput_metricparameters_set_empty_data_string(
		UserMetricsInputMetricParameters p, const char *emptyDataString) {
	try {
		MetricParameters *metricParameters(
				reinterpret_cast<MetricParameters*>(p));
		metricParameters->emptyDataString(emptyDataString);
	} catch (exception &e) {
		fprintf(stderr, "Error setting empty data string: %s\n", e.what());
	}
}

void usermetricsinput_metricparameters_set_text_domain(
		UserMetricsInputMetricParameters p, const char *textDomain) {
	try {
		MetricParameters *metricParameters(
				reinterpret_cast<MetricParameters*>(p));
		metricParameters->textDomain(textDomain);
	} catch (exception &e) {
		fprintf(stderr, "Error setting empty data string: %s\n", e.what());
	}
}

void usermetricsinput_metricparameters_set_minimum(
		UserMetricsInputMetricParameters p, double minimum) {
	try {
		MetricParameters *metricParameters(
				reinterpret_cast<MetricParameters*>(p));
		metricParameters->minimum(minimum);
	} catch (exception &e) {
		fprintf(stderr, "Error setting minimum: %s\n", e.what());
	}
}

void usermetricsinput_metricparameters_set_maximum(
		UserMetricsInputMetricParameters p, double maximum) {
	try {
		MetricParameters *metricParameters(
				reinterpret_cast<MetricParameters*>(p));
		metricParameters->maximum(maximum);
	} catch (exception &e) {
		fprintf(stderr, "Error setting maximum: %s\n", e.what());
	}
}

void usermetricsinput_metricparameters_set_type(
		UserMetricsInputMetricParameters p, USERMETRICSINPUT_METRICTYPE t) {
	try {
		MetricParameters *metricParameters(
				reinterpret_cast<MetricParameters*>(p));
		MetricType type(USER);
		if (t == METRIC_TYPE_SYSTEM) {
			type = SYSTEM;
		}
		metricParameters->type(type);
	} catch (exception &e) {
		fprintf(stderr, "Error setting maximum: %s\n", e.what());
	}
}

void usermetricsinput_metricparameters_delete(
		UserMetricsInputMetricParameters metricParameters) {
	try {
		delete reinterpret_cast<MetricParameters*>(metricParameters);
	} catch (exception &e) {
		fprintf(stderr, "Error deleting MetricParameters: %s\n", e.what());
	}
}

UserMetricsInputMetric usermetricsinput_metricmanager_add(
		UserMetricsInputMetricManager m, UserMetricsInputMetricParameters p) {
	try {
		MetricManager *metricManager(reinterpret_cast<MetricManager*>(m));
		MetricParameters *metricParameters(
				reinterpret_cast<MetricParameters*>(p));
		MetricPtr metric(metricManager->add(*metricParameters));
		return reinterpret_cast<UserMetricsInputMetric>(metric.data());
	} catch (exception &e) {
		fprintf(stderr, "Error adding Metric: %s\n", e.what());
	}
	return nullptr;
}

void usermetricsinput_metric_increment(UserMetricsInputMetric m, double amount,
		const char *username) {
	try {
		Metric *metric(reinterpret_cast<Metric*>(m));
		metric->increment(amount, username);
	} catch (exception &e) {
		fprintf(stderr, "Error incrementing Metric: %s\n", e.what());
	}
}

void usermetricsinput_metric_update_today(UserMetricsInputMetric m,
		double value, const char *username) {
	try {
		Metric *metric(reinterpret_cast<Metric*>(m));
		metric->update(value, username);
	} catch (exception &e) {
		fprintf(stderr, "Error incrementing Metric: %s\n", e.what());
	}
}

UserMetricsInputMetricUpdate usermetricsinput_metric_update(
		UserMetricsInputMetric m, const char *username) {
	try {
		Metric *metric(reinterpret_cast<Metric*>(m));
		MetricUpdatePtr metricUpdate(
				metric->update(QString::fromUtf8(username)));
		return reinterpret_cast<UserMetricsInputMetric>(metricUpdate.take());
	} catch (exception &e) {
		fprintf(stderr, "Error creating MetricUpdate: %s\n", e.what());
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
		double data) {
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
