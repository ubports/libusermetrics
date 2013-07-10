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

	UserMetricsInputMetric metric = usermetricsinput_metricmanager_add(
			metricManager, "data-source-id-capi", "format string c-api %1",
			"empty data string", "text domain");

	UserMetricsInputMetricUpdate metricUpdate = usermetricsinput_metric_update(
			metric, "username_capi");
	usermetricsinput_metricupdate_add_data(metricUpdate, 1.0);
	usermetricsinput_metricupdate_add_null(metricUpdate);
	usermetricsinput_metricupdate_add_data(metricUpdate, 0.1);
	usermetricsinput_metricupdate_delete(metricUpdate);

	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), *connection);

	{
		QList<QDBusObjectPath> dataSources = userMetricsInterface.dataSources();
		EXPECT_EQ(1, dataSources.size());
		ASSERT_EQ(DBusPaths::dataSource(1), dataSources.first().path());
	}

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1), *connection);
	EXPECT_EQ(QString("data-source-id-capi"), dataSourceInterface.name());
	EXPECT_EQ(QString("format string c-api %1"),
			dataSourceInterface.formatString());
	EXPECT_EQ(QString("empty data string"),
			dataSourceInterface.emptyDataString());
	EXPECT_EQ(QString("text domain"), dataSourceInterface.textDomain());

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1), *connection);
	EXPECT_EQ(QString("username_capi"), userDataInterface.username());

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1), *connection);

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
