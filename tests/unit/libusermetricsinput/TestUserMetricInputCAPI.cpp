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
#include <libusermetricscommon/UserMetricsInterface.h>
#include <libusermetricscommon/UserDataInterface.h>
#include <libusermetricscommon/DataSetInterface.h>
#include <libusermetricscommon/DataSourceInterface.h>
#include <libusermetricscommon/DBusPaths.h>

#include <testutils/DBusTest.h>
#include <testutils/QVariantListPrinter.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;
using namespace UserMetricsCommon;
using namespace UserMetricsTestUtils;

namespace {

class TestUserMetricInputCAPI: public DBusTest {
protected:
	TestUserMetricInputCAPI() {
	}

	virtual ~TestUserMetricInputCAPI() {
	}
};

TEST_F(TestUserMetricInputCAPI, TestBasicFunctionality) {
	UserMetricsInputMetricManager metricManager =
			usermetricsinput_metricmanager_new();

	UserMetricsInputMetricParameters p1 = usermetricsinput_metricparameters_new(
			"data-source-id-capi");
	usermetricsinput_metricparameters_set_format_string(p1,
			"format string c-api %1");
	usermetricsinput_metricparameters_set_empty_data_string(p1,
			"empty data string");
	usermetricsinput_metricparameters_set_text_domain(p1, "text domain");

	UserMetricsInputMetric metric = usermetricsinput_metricmanager_add(
			metricManager, p1);
	usermetricsinput_metricparameters_delete(p1);

	UserMetricsInputMetricParameters p2 = usermetricsinput_metricparameters_new(
			"data-source-id-capi-min");
	usermetricsinput_metricparameters_set_format_string(p2,
			"format string c-api %1 min");
	usermetricsinput_metricparameters_set_empty_data_string(p2,
			"empty data string min");
	usermetricsinput_metricparameters_set_text_domain(p2, "text domain min");
	usermetricsinput_metricparameters_set_minimum(p2, -4.0);
	usermetricsinput_metricparameters_set_maximum(p2, 5.0);
	usermetricsinput_metricparameters_set_type(p2, METRIC_TYPE_SYSTEM);

	usermetricsinput_metricmanager_add(metricManager, p2);
	usermetricsinput_metricparameters_delete(p2);

	UserMetricsInputMetricUpdate metricUpdate = usermetricsinput_metric_update(
			metric, "username_capi");
	usermetricsinput_metricupdate_add_data(metricUpdate, 1.0);
	usermetricsinput_metricupdate_add_null(metricUpdate);
	usermetricsinput_metricupdate_add_data(metricUpdate, 0.1);
	usermetricsinput_metricupdate_delete(metricUpdate);

	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), systemConnection());

	{
		QList<QDBusObjectPath> dataSources = userMetricsInterface.dataSources();
		EXPECT_EQ(2, dataSources.size());
		ASSERT_EQ(DBusPaths::dataSource(1), dataSources.at(0).path());
		ASSERT_EQ(DBusPaths::dataSource(2), dataSources.at(1).path());
	}

	{
		com::canonical::usermetrics::DataSource dataSourceInterface(
				DBusPaths::serviceName(), DBusPaths::dataSource(1),
				systemConnection());
		EXPECT_EQ(QString("data-source-id-capi"), dataSourceInterface.name());
		EXPECT_EQ(QString("format string c-api %1"),
				dataSourceInterface.formatString());
		EXPECT_EQ(QString("empty data string"),
				dataSourceInterface.emptyDataString());
		EXPECT_EQ(QString("text domain"), dataSourceInterface.textDomain());
		EXPECT_EQ(METRIC_TYPE_USER, dataSourceInterface.metricType());
	}

	{
		com::canonical::usermetrics::DataSource dataSourceInterface(
				DBusPaths::serviceName(), DBusPaths::dataSource(2),
				systemConnection());
		EXPECT_EQ(QString("data-source-id-capi-min"),
				dataSourceInterface.name());
		QVariantMap options;
		options["minimum"] = -4.0;
		options["maximum"] = 5.0;
		EXPECT_EQ(options, dataSourceInterface.options());
		EXPECT_EQ(METRIC_TYPE_SYSTEM, dataSourceInterface.metricType());
	}

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());
	EXPECT_EQ(QString("username_capi"), userDataInterface.username());

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1),
			systemConnection());

	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(3, data.size());
		EXPECT_FLOAT_EQ(1.0, data.at(0).toDouble());
		EXPECT_EQ(QString(""), data.at(1).toString());
		EXPECT_FLOAT_EQ(0.1, data.at(2).toDouble());
	}

	usermetricsinput_metric_increment(metric, 4.5, "username_capi");

	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(3, data.size());
		EXPECT_FLOAT_EQ(5.5, data.at(0).toDouble());
		EXPECT_EQ(QString(""), data.at(1).toString());
		EXPECT_FLOAT_EQ(0.1, data.at(2).toDouble());
	}

	usermetricsinput_metric_update_today(metric, -3.5, "username_capi");

	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(3, data.size());
		EXPECT_FLOAT_EQ(-3.5, data.at(0).toDouble());
		EXPECT_EQ(QString(""), data.at(1).toString());
		EXPECT_FLOAT_EQ(0.1, data.at(2).toDouble());
	}

	usermetricsinput_metricmanager_delete(metricManager);
}

} // namespace
