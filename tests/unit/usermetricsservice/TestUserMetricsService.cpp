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

#include <usermetricsservice/DBusUserMetrics.h>
#include <usermetricsservice/DBusDataSource.h>
#include <usermetricsservice/DBusUserData.h>
#include <usermetricsservice/DBusDataSet.h>

#include <testutils/QStringPrinter.h>

#include <QDjango.h>

#include <QSqlDatabase>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;
using namespace UserMetricsService;

namespace {

static const QString bus = "unix:path=/tmp/usermetricsservice-test";

class TestUserMetricsService: public Test {
protected:
	TestUserMetricsService() :
			db(
					QSqlDatabase::addDatabase("QSQLITE",
							"test-user-metrics-service")), connection(
					QDBusConnection::connectToBus(bus, "test-connection")) {
		db.setDatabaseName(":memory:");
		db.open();

//		QDjango::setDebugEnabled(true);
		QDjango::setDatabase(db);
	}

	virtual ~TestUserMetricsService() {
		db.close();
		QSqlDatabase::removeDatabase("test-user-metrics-service");
	}

	QSqlDatabase db;

	QDBusConnection connection;
};

TEST_F(TestUserMetricsService, PersistsDataSourcesBetweenRestart) {
	{
		DBusUserMetrics userMetrics(connection);

		EXPECT_TRUE(userMetrics.dataSources().empty());

		EXPECT_EQ(QString("/com/canonical/UserMetrics/DataSource/facebook"),
				userMetrics.createDataSource("facebook", "%1 messages received").path());

		DBusDataSourcePtr facebook(userMetrics.dataSource("facebook"));
		EXPECT_EQ(QString("facebook"), facebook->name());

		{
			QList<QDBusObjectPath> dataSources(userMetrics.dataSources());
			EXPECT_EQ(1, dataSources.size());
			EXPECT_EQ(QString("/com/canonical/UserMetrics/DataSource/facebook"),
					dataSources.first().path());
		}
	}

	{
		DBusUserMetrics userMetrics(connection);

		{
			QList<QDBusObjectPath> dataSources(userMetrics.dataSources());
			EXPECT_EQ(1, dataSources.size());
			EXPECT_EQ(QString("/com/canonical/UserMetrics/DataSource/facebook"),
					dataSources.first().path());
		}

		DBusDataSourcePtr facebook(userMetrics.dataSource("facebook"));
		EXPECT_EQ(QString("facebook"), facebook->name());
	}
}

TEST_F(TestUserMetricsService, PersistsUserDataBetweenRestart) {
	{
		DBusUserMetrics userMetrics(connection);

		EXPECT_TRUE(userMetrics.dataSources().empty());

		EXPECT_EQ(QString("/com/canonical/UserMetrics/UserData/alice"),
				userMetrics.createUserData("alice").path());

		DBusUserDataPtr alice(userMetrics.userData("alice"));
		EXPECT_EQ(QString("alice"), alice->username());

		{
			QList<QDBusObjectPath> userData(userMetrics.userData());
			EXPECT_EQ(1, userData.size());
			EXPECT_EQ(QString("/com/canonical/UserMetrics/UserData/alice"),
					userData.first().path());
		}
	}

	{
		DBusUserMetrics userMetrics(connection);

		{
			QList<QDBusObjectPath> userData(userMetrics.userData());
			EXPECT_EQ(1, userData.size());
			EXPECT_EQ(QString("/com/canonical/UserMetrics/UserData/alice"),
					userData.first().path());
		}

		DBusUserDataPtr alice(userMetrics.userData("alice"));
		EXPECT_EQ(QString("alice"), alice->username());
	}
}

TEST_F(TestUserMetricsService, PersistsDataSetsBetweenRestart) {
	QVariantList data( { 100.0, 50.0, 0.0, -50.0, -100.0 });

	{
		DBusUserMetrics userMetrics(connection);

		userMetrics.createDataSource("twitter", "%1 tweets received");
		userMetrics.createUserData("alice");

		DBusUserDataPtr alice(userMetrics.userData("alice"));

		// should re-use the same data set
		EXPECT_EQ(QString("/com/canonical/UserMetrics/DataSet/1"),
				alice->createDataSet("twitter").path());;
		EXPECT_EQ(QString("/com/canonical/UserMetrics/DataSet/1"),
				alice->createDataSet("twitter").path());;

		DBusDataSetPtr twitter(alice->dataSet("twitter"));

		twitter->update(data);
	}

	{
		DBusUserMetrics userMetrics(connection);

		DBusUserDataPtr alice(userMetrics.userData("alice"));
		EXPECT_EQ(QString("alice"), alice->username());

		DBusDataSetPtr twitter(alice->dataSet("twitter"));

		EXPECT_EQ(data, twitter->data());
	}
}

} // namespace
