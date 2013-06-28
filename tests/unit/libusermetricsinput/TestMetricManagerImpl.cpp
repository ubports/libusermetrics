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

#include <libusermetricsinput/MetricManagerImpl.h>
#include <libusermetricscommon/UserMetricsInterface.h>
#include <libusermetricscommon/UserDataInterface.h>
#include <libusermetricscommon/DataSetInterface.h>
#include <libusermetricscommon/DataSourceInterface.h>
#include <libusermetricscommon/DBusPaths.h>

#include <QtCore/QDebug>

#include <testutils/DBusTest.h>
#include <testutils/QStringPrinter.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;
using namespace UserMetricsCommon;
using namespace UserMetricsInput;
using namespace UserMetricsTestUtils;

namespace {

class TestMetricManagerImpl: public DBusTest {
protected:
	TestMetricManagerImpl() {
	}

	virtual ~TestMetricManagerImpl() {
	}
};

TEST_F(TestMetricManagerImpl, TestCanAddDataSourceMultipleTimes) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), *connection);
	{
		QList<QDBusObjectPath> dataSources = userMetricsInterface.dataSources();
		EXPECT_TRUE(dataSources.empty());
	}

	{
		MetricManagerPtr manager(new MetricManagerImpl(*connection));
		MetricPtr metric(manager->add("data-source-id", "format string %1"));
	}

	{
		MetricManagerPtr manager(new MetricManagerImpl(*connection));
		MetricPtr metric(manager->add("data-source-id", "format string %1"));
	}

	{
		QList<QDBusObjectPath> dataSources = userMetricsInterface.dataSources();
		EXPECT_EQ(1, dataSources.size());
		EXPECT_EQ(DBusPaths::dataSource(1), dataSources.first().path());
	}

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1), *connection);
	QString name(dataSourceInterface.name());
	EXPECT_EQ(QString("data-source-id"), name);
}

TEST_F(TestMetricManagerImpl, TestCanAddDataAndUpdate) {
	MetricManagerPtr manager(new MetricManagerImpl(*connection));

	MetricPtr metric(manager->add("data-source-id", "format string %1"));

	{
		MetricUpdatePtr update(metric->update("the-username"));
		update->addData(100.0);
		update->addNull();
		update->addData(-50.0);
		update->addData(-22.0);
	}

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1), *connection);
	EXPECT_EQ(QString("the-username"), userDataInterface.username());

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1), *connection);
	QVariantList data(dataSetInterface.data());
	EXPECT_FLOAT_EQ(100.0, data.at(0).toDouble());
	EXPECT_EQ(QString(""), data.at(1).toString());
	EXPECT_FLOAT_EQ(-50.0, data.at(2).toDouble());
	EXPECT_FLOAT_EQ(-22.0, data.at(3).toDouble());
}

} // namespace
