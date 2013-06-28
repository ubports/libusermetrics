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

//	{
//		QList<QDBusObjectPath> dataSources = userMetricsInterface.dataSources();
//		ASSERT_EQ(1, dataSources.size());
//		EXPECT_EQ(DBusPaths::dataSource(1), dataSources.first().path());
//	}

//	com::canonical::usermetrics::DataSource dataSourceInterface(
//			DBusPaths::serviceName(), DBusPaths::dataSource(1), *connection);
//	EXPECT_EQ(QString("data-source-id-capi"), dataSourceInterface.name());
//	EXPECT_EQ(QString("format string c-api %1"),
//			dataSourceInterface.formatString());
//
//	com::canonical::usermetrics::UserData userDataInterface(
//			DBusPaths::serviceName(), DBusPaths::userData(1), *connection);
//	EXPECT_EQ(QString("username_capi"), userDataInterface.username());
//
//	com::canonical::usermetrics::DataSet dataSetInterface(
//			DBusPaths::serviceName(), DBusPaths::dataSet(1), *connection);
//	QVariantList data(dataSetInterface.data());
//	ASSERT_EQ(3, data.size());
//	EXPECT_FLOAT_EQ(1.0, data.at(0).toDouble());
//	EXPECT_EQ(QString(""), data.at(1).toString());
//	EXPECT_FLOAT_EQ(0.1, data.at(2).toDouble());

}// namespace
