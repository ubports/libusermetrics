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

#include <QtTest/QSignalSpy>

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

TEST_F(TestSyncedUserMetricsStore, LoadsDataSourcesAtStartup) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), *connection);

	QDBusObjectPath dataSourcePath1(
			userMetricsInterface.createDataSource("data-source-one",
					"format string one %1"));
	ASSERT_EQ(DBusPaths::dataSource(1), dataSourcePath1.path());

	QDBusObjectPath dataSourcePath2(
			userMetricsInterface.createDataSource("data-source-two",
					"format string two %1"));
	ASSERT_EQ(DBusPaths::dataSource(2), dataSourcePath2.path());

	SyncedUserMetricsStore store(*connection);

	{
		DataSourcePtr dataSource(store.dataSource("data-source-one"));
		EXPECT_EQ(QString("format string one %1"), dataSource->formatString());
	}

	{
		DataSourcePtr dataSource(store.dataSource("data-source-two"));
		EXPECT_EQ(QString("format string two %1"), dataSource->formatString());
	}
}

TEST_F(TestSyncedUserMetricsStore, SyncsNewDataSources) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), *connection);

	QDBusObjectPath dataSourcePath1(
			userMetricsInterface.createDataSource("data-source-one",
					"format string one %1"));
	ASSERT_EQ(DBusPaths::dataSource(1), dataSourcePath1.path());

	SyncedUserMetricsStore store(*connection);

	{
		DataSourcePtr dataSource(store.dataSource("data-source-one"));
		ASSERT_FALSE(dataSource.isNull());
		EXPECT_EQ(QString("format string one %1"), dataSource->formatString());
	}

	{
		DataSourcePtr dataSource(store.dataSource("data-source-two"));
		ASSERT_TRUE(dataSource.isNull());
	}

	QSignalSpy spy(&userMetricsInterface,
			SIGNAL(dataSourceAdded(const QString &, const QDBusObjectPath &)));

	QDBusObjectPath dataSourcePath2(
			userMetricsInterface.createDataSource("data-source-two",
					"format string two %1"));
	ASSERT_EQ(DBusPaths::dataSource(2), dataSourcePath2.path());

	spy.wait();

	{
		DataSourcePtr dataSource(store.dataSource("data-source-one"));
		ASSERT_FALSE(dataSource.isNull());
		EXPECT_EQ(QString("format string one %1"), dataSource->formatString());
	}

	{
		DataSourcePtr dataSource(store.dataSource("data-source-two"));
		ASSERT_FALSE(dataSource.isNull());
		EXPECT_EQ(QString("format string two %1"), dataSource->formatString());
	}
}

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
		UserMetricsStore::const_iterator it(store.constFind("username1"));
		ASSERT_NE(it, store.constEnd());
		EXPECT_EQ(QString("username1"), it.key());
	}

	{
		UserMetricsStore::const_iterator it(store.constFind("username2"));
		ASSERT_NE(it, store.constEnd());
		EXPECT_EQ(QString("username2"), it.key());
	}
}

TEST_F(TestSyncedUserMetricsStore, SyncsNewUserData) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), *connection);

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username1"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	SyncedUserMetricsStore store(*connection);

	{
		UserMetricsStore::const_iterator it(store.constFind("username1"));
		ASSERT_NE(it, store.constEnd());
		EXPECT_EQ(QString("username1"), it.key());
	}

	{
		UserMetricsStore::const_iterator it(store.constFind("username2"));
		ASSERT_EQ(it, store.constEnd());
	}

	QSignalSpy spy(&userMetricsInterface,
			SIGNAL(userDataAdded(const QString &, const QDBusObjectPath &)));

	QDBusObjectPath userDataPath2(
			userMetricsInterface.createUserData("username2"));
	ASSERT_EQ(DBusPaths::userData(2), userDataPath2.path());

	spy.wait();

	{
		UserMetricsStore::const_iterator it(store.constFind("username1"));
		ASSERT_NE(it, store.constEnd());
		EXPECT_EQ(QString("username1"), it.key());
	}

	{
		UserMetricsStore::const_iterator it(store.constFind("username2"));
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
	QVariantList expected( { 1.0, 0.75, QVariant(), 0.25, 0.0 });

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1), *connection);
	dataSetInterface.update(data);

	SyncedUserMetricsStore store(*connection);

	UserMetricsStore::const_iterator userDataIterator(
			store.constFind("username"));
	ASSERT_NE(userDataIterator, store.constEnd());
	EXPECT_EQ(QString("username"), userDataIterator.key());
	UserDataPtr userData(*userDataIterator);

	UserData::const_iterator dataSetIterator(userData->constBegin());
	ASSERT_NE(dataSetIterator, userData->constEnd());
	DataSetPtr dataSet(*dataSetIterator);
	EXPECT_EQ(expected, dataSet->data());
	EXPECT_EQ(QDate::currentDate(), dataSet->lastUpdated());
}

TEST_F(TestSyncedUserMetricsStore, SyncsNewDataSets) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), *connection);

	QDBusObjectPath twitterPath(
			userMetricsInterface.createDataSource("twitter",
					"twitter format string"));
	ASSERT_EQ(DBusPaths::dataSource(1), twitterPath.path());

	QDBusObjectPath facebookPath(
			userMetricsInterface.createDataSource("facebook",
					"facebook format string"));
	ASSERT_EQ(DBusPaths::dataSource(2), facebookPath.path());

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1), *connection);

	QDBusObjectPath twitterDataPath(userDataInterface.createDataSet("twitter"));
	ASSERT_EQ(DBusPaths::dataSet(1), twitterDataPath.path());

	SyncedUserMetricsStore store(*connection);

	{
		UserMetricsStore::const_iterator userDataIterator(
				store.constFind("username"));
		ASSERT_NE(userDataIterator, store.constEnd());
		EXPECT_EQ(QString("username"), userDataIterator.key());
		UserDataPtr userData(*userDataIterator);

		UserData::const_iterator dataSetIterator(userData->constBegin());
		ASSERT_NE(dataSetIterator, userData->constEnd());
		EXPECT_EQ(QString("twitter"), dataSetIterator.key());

		++dataSetIterator;
		ASSERT_EQ(dataSetIterator, userData->constEnd());
	}

	QSignalSpy spy(&userDataInterface,
			SIGNAL(dataSetAdded(const QString &, const QDBusObjectPath &)));

	QDBusObjectPath facebookDataPath(
			userDataInterface.createDataSet("facebook"));
	ASSERT_EQ(DBusPaths::dataSet(2), facebookDataPath.path());

	spy.wait();

	{
		UserMetricsStore::const_iterator userDataIterator(
				store.constFind("username"));
		ASSERT_NE(userDataIterator, store.constEnd());
		EXPECT_EQ(QString("username"), userDataIterator.key());
		UserDataPtr userData(*userDataIterator);

		UserData::const_iterator dataSetIterator(userData->constBegin());

		ASSERT_NE(dataSetIterator, userData->constEnd());
		EXPECT_EQ(QString("facebook"), dataSetIterator.key());

		++dataSetIterator;
		ASSERT_NE(dataSetIterator, userData->constEnd());
		EXPECT_EQ(QString("twitter"), dataSetIterator.key());

		++dataSetIterator;
		ASSERT_EQ(dataSetIterator, userData->constEnd());
	}
}

} // namespace
