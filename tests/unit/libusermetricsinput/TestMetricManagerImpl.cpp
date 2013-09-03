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
			DBusPaths::userMetrics(), systemConnection());
	{
		QList<QDBusObjectPath> dataSources = userMetricsInterface.dataSources();
		EXPECT_TRUE(dataSources.empty());
	}

	{
		MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));
		MetricPtr metric(manager->add("data-source-id", "format string %1"));
	}

	{
		MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));
		MetricPtr metric(manager->add("data-source-id", "format string %1"));
	}

	{
		QList<QDBusObjectPath> dataSources = userMetricsInterface.dataSources();
		ASSERT_EQ(1, dataSources.size());
		EXPECT_EQ(DBusPaths::dataSource(1), dataSources.first().path());
	}

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1),
			systemConnection());
	QString name(dataSourceInterface.name());
	EXPECT_EQ(QString("data-source-id"), name);
}

TEST_F(TestMetricManagerImpl, TestCanAddDataAndUpdate) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(manager->add("data-source-id", "format string %1"));

	{
		MetricUpdatePtr update(metric->update("the-username"));
		update->addData(100.0);
		update->addNull();
		update->addData(-50.0);
		update->addData(-22.0);
	}

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());
	EXPECT_EQ(QString("the-username"), userDataInterface.username());

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1),
			systemConnection());
	QVariantList data(dataSetInterface.data());
	EXPECT_FLOAT_EQ(100.0, data.at(0).toDouble());
	EXPECT_EQ(QString(""), data.at(1).toString());
	EXPECT_FLOAT_EQ(-50.0, data.at(2).toDouble());
	EXPECT_FLOAT_EQ(-22.0, data.at(3).toDouble());

	QDateTime dateTime(QDateTime::fromTime_t(dataSetInterface.lastUpdated()));
	EXPECT_EQ(QDate::currentDate(), dateTime.date());
}

TEST_F(TestMetricManagerImpl, TestAddMultipleDataSourcesAndUsers) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metricOne(
			manager->add("data-source-one", "format string one %1"));
	MetricPtr metricTwo(
			manager->add("data-source-two", "format string two %1"));

	{
		MetricUpdatePtr updateOne(metricOne->update("the-username-one"));
		updateOne->addData(100.0);
		MetricUpdatePtr updateTwo(metricOne->update("the-username-two"));
		updateTwo->addData(75.0);

		MetricUpdatePtr updateThree(metricTwo->update("the-username-one"));
		updateThree->addData(50.0);
		MetricUpdatePtr updateFour(metricTwo->update("the-username-two"));
		updateFour->addData(25.0);
	}

	com::canonical::usermetrics::UserData userDataInterfaceOne(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());
	EXPECT_EQ(QString("the-username-one"), userDataInterfaceOne.username());
	com::canonical::usermetrics::UserData userDataInterfaceTwo(
			DBusPaths::serviceName(), DBusPaths::userData(2),
			systemConnection());
	EXPECT_EQ(QString("the-username-two"), userDataInterfaceTwo.username());

	com::canonical::usermetrics::DataSet dataSetInterfaceOne(
			DBusPaths::serviceName(), DBusPaths::dataSet(1),
			systemConnection());
	QVariantList dataOne(dataSetInterfaceOne.data());
	EXPECT_FLOAT_EQ(100.0, dataOne.first().toDouble());

	com::canonical::usermetrics::DataSet dataSetInterfaceTwo(
			DBusPaths::serviceName(), DBusPaths::dataSet(2),
			systemConnection());
	QVariantList dataTwo(dataSetInterfaceTwo.data());
	EXPECT_FLOAT_EQ(75.0, dataTwo.first().toDouble());

	com::canonical::usermetrics::DataSet dataSetInterfaceThree(
			DBusPaths::serviceName(), DBusPaths::dataSet(3),
			systemConnection());
	QVariantList dataThree(dataSetInterfaceThree.data());
	EXPECT_FLOAT_EQ(50.0, dataThree.first().toDouble());

	com::canonical::usermetrics::DataSet dataSetInterfaceFour(
			DBusPaths::serviceName(), DBusPaths::dataSet(4),
			systemConnection());
	QVariantList dataFour(dataSetInterfaceFour.data());
	EXPECT_FLOAT_EQ(25.0, dataFour.first().toDouble());
}

TEST_F(TestMetricManagerImpl, TestCanAddDataAndUpdateToday) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(manager->add("data-source-id", "format string %1"));
	metric->update(2.5, "the-username");

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());
	EXPECT_EQ(QString("the-username"), userDataInterface.username());

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1),
			systemConnection());
	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(1, data.size());
		EXPECT_FLOAT_EQ(2.5, data.at(0).toDouble());
	}

	metric->update(1.3, "the-username");
	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(1, data.size());
		EXPECT_FLOAT_EQ(1.3, data.at(0).toDouble());
	}

	QDateTime dateTime(QDateTime::fromTime_t(dataSetInterface.lastUpdated()));
	EXPECT_EQ(QDate::currentDate(), dateTime.date());
}

TEST_F(TestMetricManagerImpl, TestCanAddDataAndIncrement) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(manager->add("data-source-id", "format string %1"));
	metric->increment(1.0, "the-username");

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());
	EXPECT_EQ(QString("the-username"), userDataInterface.username());

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1),
			systemConnection());
	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(1, data.size());
		EXPECT_FLOAT_EQ(1.0, data.at(0).toDouble());
	}

	metric->increment(2.0, "the-username");
	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(1, data.size());
		EXPECT_FLOAT_EQ(3.0, data.at(0).toDouble());
	}

	QDateTime dateTime(QDateTime::fromTime_t(dataSetInterface.lastUpdated()));
	EXPECT_EQ(QDate::currentDate(), dateTime.date());
}

TEST_F(TestMetricManagerImpl, TestCanAddNullAndIncrement) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(manager->add("data-source-id", "format string %1"));
	{
		MetricUpdatePtr update(metric->update("the-username"));
		update->addNull();
	}

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());
	EXPECT_EQ(QString("the-username"), userDataInterface.username());

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1),
			systemConnection());
	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(1, data.size());
		EXPECT_EQ(QString(""), data.at(0).toString());
	}

	metric->increment(5.2, "the-username");
	{
		QVariantList data(dataSetInterface.data());
		ASSERT_EQ(1, data.size());
		EXPECT_FLOAT_EQ(5.2, data.at(0).toDouble());
	}

	QDateTime dateTime(QDateTime::fromTime_t(dataSetInterface.lastUpdated()));
	EXPECT_EQ(QDate::currentDate(), dateTime.date());
}

TEST_F(TestMetricManagerImpl, TestMinimum) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(
			manager->add(
					MetricParameters("data-source-id").formatString(
							"format string %1").minimum(-5.0)));

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1),
			systemConnection());

	QVariantMap expected;
	expected["minimum"] = -5.0;
	EXPECT_EQ(expected, dataSourceInterface.options());
}

TEST_F(TestMetricManagerImpl, TestMaximum) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(
			manager->add(
					MetricParameters("data-source-id").formatString(
							"format string %1").maximum(4.0)));

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1),
			systemConnection());

	QVariantMap expected;
	expected["maximum"] = 4.0;
	EXPECT_EQ(expected, dataSourceInterface.options());
}

TEST_F(TestMetricManagerImpl, TestMiniumMaximum) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(
			manager->add(
					MetricParameters("data-source-id").formatString(
							"format string %1").minimum(1.0).maximum(5.0)));

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1),
			systemConnection());

	QVariantMap expected;
	expected["minimum"] = 1.0;
	expected["maximum"] = 5.0;
	EXPECT_EQ(expected, dataSourceInterface.options());
}

TEST_F(TestMetricManagerImpl, TestDefaultType) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(
			manager->add(
					MetricParameters("data-source-id").formatString(
							"format string %1")));

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1),
			systemConnection());

	EXPECT_EQ(MetricType::USER, dataSourceInterface.metricType());
}

TEST_F(TestMetricManagerImpl, TestUserType) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(
			manager->add(
					MetricParameters("data-source-id").formatString(
							"format string %1").type(MetricType::USER)));

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1),
			systemConnection());

	EXPECT_EQ(MetricType::USER, dataSourceInterface.metricType());
}

TEST_F(TestMetricManagerImpl, TestSystemType) {
	MetricManagerPtr manager(new MetricManagerImpl(systemConnection()));

	MetricPtr metric(
			manager->add(
					MetricParameters("data-source-id").formatString(
							"format string %1").type(MetricType::SYSTEM)));

	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(1),
			systemConnection());

	EXPECT_EQ(MetricType::SYSTEM, dataSourceInterface.metricType());
}

} // namespace
