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

#include <libusermetricsoutput/SyncedUserMetricsStore.h>
#include <libusermetricscommon/UserMetricsInterface.h>
#include <libusermetricscommon/UserDataInterface.h>
#include <libusermetricscommon/DataSetInterface.h>
#include <libusermetricscommon/DataSourceInterface.h>
#include <libusermetricscommon/DBusPaths.h>

#include <testutils/DBusTest.h>
#include <testutils/QStringPrinter.h>
#include <testutils/QVariantPrinter.h>
#include <testutils/QVariantListPrinter.h>

#include <gtest/gtest.h>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsOutput;
using namespace UserMetricsTestUtils;

namespace {

class TestSyncedUserMetricsStore: public DBusTest {
protected:
	TestSyncedUserMetricsStore() {
	}

	virtual ~TestSyncedUserMetricsStore() {
	}
};

TEST_F(TestSyncedUserMetricsStore, LoadsUserDataAtStartup) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), *connection);

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username1"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	QDBusObjectPath userDataPath2(
			userMetricsInterface.createUserData("username2"));
	ASSERT_EQ(DBusPaths::userData(2), userDataPath2.path());

	SyncedUserMetricsStore store(*connection);

	{
		auto it(store.constFind("username1"));
		ASSERT_NE(it, store.constEnd());
		EXPECT_EQ(QString("username1"), it.key());
	}

	{
		auto it(store.constFind("username2"));
		ASSERT_NE(it, store.constEnd());
		EXPECT_EQ(QString("username2"), it.key());
	}
}

TEST_F(TestSyncedUserMetricsStore, LoadsDataSetsAtStartup) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), *connection);

	QDBusObjectPath twitterPath(
			userMetricsInterface.createDataSource("twitter",
					"twitter format string"));
	ASSERT_EQ(DBusPaths::dataSource(1), twitterPath.path());

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1), *connection);
	QDBusObjectPath twitterDataPath(userDataInterface.createDataSet("twitter"));
	ASSERT_EQ(DBusPaths::dataSet(1), twitterDataPath.path());

	QVariantList data( { 100.0, 50.0, "", -50.0, -100.0 });
	QVariantList expected( { 100.0, 50.0, QVariant(), -50.0, -100.0 });

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1), *connection);
	dataSetInterface.update(data);

	SyncedUserMetricsStore store(*connection);

	auto userDataIterator(store.constFind("username"));
	ASSERT_NE(userDataIterator, store.constEnd());
	EXPECT_EQ(QString("username"), userDataIterator.key());
	UserDataPtr userData(*userDataIterator);

	auto dataSetIterator(userData->constBegin());
	ASSERT_NE(dataSetIterator, userData->constEnd());
	DataSetPtr dataSet(*dataSetIterator);
	EXPECT_EQ(expected, dataSet->data());
	EXPECT_EQ(QDate::currentDate(), dataSet->lastUpdated());
}

//	QVariantList data(dataSetInterface.data());
//	ASSERT_EQ(3, data.size());
//	EXPECT_FLOAT_EQ(1.0, data.at(0).toDouble());
//	EXPECT_EQ(QString(""), data.at(1).toString());
//	EXPECT_FLOAT_EQ(0.1, data.at(2).toDouble());

}// namespace
