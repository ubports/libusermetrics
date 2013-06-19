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

#include <libusermetricsinput/usermetricsinput.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

namespace {

class TestUserMetricInputCAPI: public Test {
protected:
	TestUserMetricInputCAPI() {
	}

	virtual ~TestUserMetricInputCAPI() {
	}
};

TEST_F(TestUserMetricInputCAPI, Foo) {
	UserMetricsInputMetricManager metricManager =
			usermetricsinput_metricmanager_new();

	UserMetricsInputMetric metric = usermetricsinput_metricmanager_add(
			metricManager, "data-source-id", "format string %1");

	UserMetricsInputMetricUpdate metricUpdate = usermetricsinput_metric_update(
			metric, "username");
	usermetricsinput_metricupdate_add_data(metricUpdate, 1.0);
	usermetricsinput_metricupdate_add_null(metricUpdate);
	usermetricsinput_metricupdate_add_data(metricUpdate, 0.1);
	usermetricsinput_metricupdate_delete(metricUpdate);

	usermetricsinput_metricmanager_delete(metricManager);
}

} // namespace
