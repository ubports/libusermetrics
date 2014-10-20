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
#include <libusermetricsoutput/SyncedUserData.h>
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
			DBusPaths::userMetrics(), systemConnection());

	QDBusObjectPath dataSourcePath1(
			userMetricsInterface.createDataSource("data-source-one",
					"format string one %1", "empty data string one",
					"text domain one", MetricType::USER, QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(1), dataSourcePath1.path());

	QDBusObjectPath dataSourcePath2(
			userMetricsInterface.createDataSource("data-source-two",
					"format string two %1", "empty data string two",
					"text domain two", MetricType::USER, QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(2), dataSourcePath2.path());

	SyncedUserMetricsStore store(systemConnection());
	QSignalSpy connectionEstablishedSpy(&store,
			SIGNAL(connectionEstablished()));
	connectionEstablishedSpy.wait();

	{
		DataSourcePtr dataSource(store.dataSource(DBusPaths::dataSource(1)));
		ASSERT_FALSE(dataSource.isNull());
		QSignalSpy emptyDataStringChangedSpy(dataSource.data(),
				SIGNAL(emptyDataStringChanged(const QString &)));
		ASSERT_TRUE(emptyDataStringChangedSpy.wait());
		EXPECT_EQ(QString("format string one %1"), dataSource->formatString());
		EXPECT_EQ(QString("empty data string one"),
				dataSource->emptyDataString());
		EXPECT_EQ(QString("text domain one"), dataSource->textDomain());
	}

	{
		DataSourcePtr dataSource(store.dataSource(DBusPaths::dataSource(2)));
		ASSERT_FALSE(dataSource.isNull());
		QSignalSpy emptyDataStringChangedSpy(dataSource.data(),
				SIGNAL(emptyDataStringChanged(const QString &)));
		ASSERT_TRUE(emptyDataStringChangedSpy.wait());
		EXPECT_EQ(QString("format string two %1"), dataSource->formatString());
		EXPECT_EQ(QString("empty data string two"),
				dataSource->emptyDataString());
		EXPECT_EQ(QString("text domain two"), dataSource->textDomain());
	}
}

TEST_F(TestSyncedUserMetricsStore, SyncsNewDataSources) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), systemConnection());

	QDBusObjectPath dataSourcePath1(
			userMetricsInterface.createDataSource("data-source-one",
					"format string one %1", "no data source ones",
					"text-domain-one", MetricType::USER, QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(1), dataSourcePath1.path());

	SyncedUserMetricsStore store(systemConnection());
	QSignalSpy connectionEstablishedSpy(&store,
			SIGNAL(connectionEstablished()));
	connectionEstablishedSpy.wait();

	{
		DataSourcePtr dataSource(store.dataSource(DBusPaths::dataSource(1)));
		ASSERT_FALSE(dataSource.isNull());
		QSignalSpy formatStringChangedSpy(dataSource.data(),
				SIGNAL(formatStringChanged(const QString &)));
		ASSERT_TRUE(formatStringChangedSpy.wait());
		EXPECT_EQ(QString("format string one %1"), dataSource->formatString());
		EXPECT_EQ(QString("text-domain-one"), dataSource->textDomain());
	}

	{
		DataSourcePtr dataSource(store.dataSource("data-source-two"));
		ASSERT_TRUE(dataSource.isNull());
	}

	QSignalSpy spy(&userMetricsInterface,
			SIGNAL(dataSourceAdded(const QDBusObjectPath &)));

	QDBusObjectPath dataSourcePath2(
			userMetricsInterface.createDataSource("data-source-two",
					"format string two %1", "no data source twos",
					"text-domain-two", MetricType::USER, QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(2), dataSourcePath2.path());

	spy.wait();

	{
		DataSourcePtr dataSource(store.dataSource(DBusPaths::dataSource(1)));
		ASSERT_FALSE(dataSource.isNull());
		if (dataSource->emptyDataString().isEmpty()) {
			QSignalSpy emptyDataStringChangedSpy(dataSource.data(),
					SIGNAL(emptyDataStringChanged(const QString &)));
			ASSERT_TRUE(emptyDataStringChangedSpy.wait());
		}
		EXPECT_EQ(QString("format string one %1"), dataSource->formatString());
		EXPECT_EQ(QString("no data source ones"),
				dataSource->emptyDataString());
		EXPECT_EQ(QString("text-domain-one"), dataSource->textDomain());
	}

	{
		DataSourcePtr dataSource(store.dataSource(DBusPaths::dataSource(2)));
		ASSERT_FALSE(dataSource.isNull());
		QSignalSpy emptyDataStringChangedSpy(dataSource.data(),
				SIGNAL(emptyDataStringChanged(const QString &)));
		ASSERT_TRUE(emptyDataStringChangedSpy.wait());
		EXPECT_EQ(QString("format string two %1"), dataSource->formatString());
		EXPECT_EQ(QString("no data source twos"),
				dataSource->emptyDataString());
		EXPECT_EQ(QString("text-domain-two"), dataSource->textDomain());
	}
}

TEST_F(TestSyncedUserMetricsStore, LoadsUserDataAtStartup) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), systemConnection());

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username1"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	QDBusObjectPath userDataPath2(
			userMetricsInterface.createUserData("username2"));
	ASSERT_EQ(DBusPaths::userData(2), userDataPath2.path());

	SyncedUserMetricsStore store(systemConnection());
	QSignalSpy connectionEstablishedSpy(&store,
			SIGNAL(connectionEstablished()));
	connectionEstablishedSpy.wait();

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
			DBusPaths::userMetrics(), systemConnection());

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username1"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	SyncedUserMetricsStore store(systemConnection());
	QSignalSpy connectionEstablishedSpy(&store,
			SIGNAL(connectionEstablished()));
	connectionEstablishedSpy.wait();

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
			DBusPaths::userMetrics(), systemConnection());

	QDBusObjectPath twitterPath(
			userMetricsInterface.createDataSource("twitter",
					"twitter format string", "", "", MetricType::USER,
					QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(1), twitterPath.path());

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());
	QDBusObjectPath twitterDataPath(userDataInterface.createDataSet("twitter"));
	ASSERT_EQ(DBusPaths::dataSet(1), twitterDataPath.path());

	QVariantList data( { 100.0, 50.0, "", -50.0, -100.0 });
	QVariantList expected( { 1.0, 0.75, QVariant(), 0.25, 0.0 });

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1),
			systemConnection());
	dataSetInterface.update(data);

	SyncedUserMetricsStore store(systemConnection());
	QSignalSpy connectionEstablishedSpy(&store,
			SIGNAL(connectionEstablished()));
	connectionEstablishedSpy.wait();

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
			DBusPaths::userMetrics(), systemConnection());

	QDBusObjectPath twitterPath(
			userMetricsInterface.createDataSource("twitter",
					"twitter format string", "no tweets today", "",
					MetricType::USER, QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(1), twitterPath.path());

	QDBusObjectPath facebookPath(
			userMetricsInterface.createDataSource("facebook",
					"facebook format string", "no facebook messages today", "",
					MetricType::USER, QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(2), facebookPath.path());

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());

	QDBusObjectPath twitterDataPath(userDataInterface.createDataSet("twitter"));
	ASSERT_EQ(DBusPaths::dataSet(1), twitterDataPath.path());

	SyncedUserMetricsStore store(systemConnection());
	QSignalSpy connectionEstablishedSpy(&store,
			SIGNAL(connectionEstablished()));
	connectionEstablishedSpy.wait();

	UserMetricsStore::const_iterator userDataIterator(
			store.constFind("username"));
	ASSERT_NE(userDataIterator, store.constEnd());
	EXPECT_EQ(QString("username"), userDataIterator.key());
	UserDataPtr userData(*userDataIterator);

	{
		UserData::const_iterator dataSetIterator(userData->constBegin());
		ASSERT_NE(dataSetIterator, userData->constEnd());
		EXPECT_EQ(QString(DBusPaths::dataSource(1)), dataSetIterator.key());

		++dataSetIterator;
		ASSERT_EQ(dataSetIterator, userData->constEnd());
	}

	QSignalSpy spy(static_cast<SyncedUserData *>(userData.data()),
			SIGNAL(dataSetAdded(const QString &)));

	QDBusObjectPath facebookDataPath(
			userDataInterface.createDataSet("facebook"));
	ASSERT_EQ(DBusPaths::dataSet(2), facebookDataPath.path());

	spy.wait();
	ASSERT_FALSE(spy.empty());

	{
		UserData::const_iterator dataSetIterator(userData->constBegin());

		ASSERT_NE(dataSetIterator, userData->constEnd());
		EXPECT_EQ(DBusPaths::dataSource(1).toStdString(),
				dataSetIterator.key().toStdString());

		++dataSetIterator;
		ASSERT_NE(dataSetIterator, userData->constEnd());
		EXPECT_EQ(DBusPaths::dataSource(2), dataSetIterator.key());

		++dataSetIterator;
		ASSERT_EQ(dataSetIterator, userData->constEnd());
	}
}

TEST_F(TestSyncedUserMetricsStore, MergesSystemDataAtStartup) {
	com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
			DBusPaths::userMetrics(), systemConnection());

	QDBusObjectPath twitterPath(
			userMetricsInterface.createDataSource("twitter",
					"twitter format string", "", "", MetricType::USER,
					QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(1), twitterPath.path());

	QDBusObjectPath batteryPath(
			userMetricsInterface.createDataSource("battery",
					"battery level format string", "", "", MetricType::SYSTEM,
					QVariantMap()));
	ASSERT_EQ(DBusPaths::dataSource(2), batteryPath.path());

	QDBusObjectPath userDataPath(
			userMetricsInterface.createUserData("username"));
	ASSERT_EQ(DBusPaths::userData(1), userDataPath.path());

	QDBusObjectPath systemDataPath(userMetricsInterface.createUserData(""));
	ASSERT_EQ(DBusPaths::userData(2), systemDataPath.path());

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(1),
			systemConnection());
	QDBusObjectPath twitterDataPath(userDataInterface.createDataSet("twitter"));
	ASSERT_EQ(DBusPaths::dataSet(1), twitterDataPath.path());

	QVariantList twitterData( { 100.0, 50.0, "", -50.0, -100.0 });
	QVariantList twitterExpected( { 1.0, 0.75, QVariant(), 0.25, 0.0 });

	com::canonical::usermetrics::DataSet twitterDataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(1),
			systemConnection());
	twitterDataSetInterface.update(twitterData);

	com::canonical::usermetrics::UserData systemDataInterface(
			DBusPaths::serviceName(), DBusPaths::userData(2),
			systemConnection());
	QDBusObjectPath batteryDataPath(
			systemDataInterface.createDataSet("battery"));
	ASSERT_EQ(DBusPaths::dataSet(2), batteryDataPath.path());

	QVariantList batteryData( { -100.0, -50.0, "", 50.0, 100.0 });
	QVariantList batteryExpected( { 0.0, 0.25, QVariant(), 0.75, 1.0 });

	com::canonical::usermetrics::DataSet batteryDataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(2),
			systemConnection());
	batteryDataSetInterface.update(batteryData);

	SyncedUserMetricsStore store(systemConnection());
	QSignalSpy connectionEstablishedSpy(&store,
			SIGNAL(connectionEstablished()));
	connectionEstablishedSpy.wait();

	UserMetricsStore::const_iterator userDataIterator(
			store.constFind("username"));
	ASSERT_NE(userDataIterator, store.constEnd());
	EXPECT_EQ(QString("username"), userDataIterator.key());
	UserDataPtr userData(*userDataIterator);

	UserData::const_iterator dataSetIterator(userData->constBegin());
	ASSERT_NE(dataSetIterator, userData->constEnd());
	// twitter data set
	{
		DataSetPtr dataSet(*dataSetIterator);
		EXPECT_EQ(twitterExpected, dataSet->data());
		EXPECT_EQ(QDate::currentDate(), dataSet->lastUpdated());
	}

	++dataSetIterator;
	ASSERT_NE(dataSetIterator, userData->constEnd());
	// battery data set
	{
		DataSetPtr dataSet(*dataSetIterator);
		EXPECT_EQ(batteryExpected, dataSet->data());
		EXPECT_EQ(QDate::currentDate(), dataSet->lastUpdated());
	}
}

} // namespace
