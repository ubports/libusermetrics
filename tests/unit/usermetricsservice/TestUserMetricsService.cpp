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
	TestUserMetricsService() {
	}

	virtual ~TestUserMetricsService() {
	}
};

TEST_F(TestUserMetricsService, PersistsDataSourcesBetweenRestart) {
	QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
	db.setDatabaseName(":memory:");
	db.open();

	QDjango::setDatabase(db);

	QDBusConnection connection(
			QDBusConnection::connectToBus(bus, "test-connection"));

	{
		DBusUserMetrics userMetrics(connection);

		EXPECT_EQ(QString("/com/canonical/UserMetrics/DataSource/facebook"),
				userMetrics.createDataSource("facebook", "%1 messages received").path());

		DBusDataSourcePtr facebook(userMetrics.dataSource("facebook"));
		EXPECT_EQ(QString("facebook"), facebook->name());
	}

	{
		DBusUserMetrics userMetrics(connection);

		DBusDataSourcePtr facebook(userMetrics.dataSource("facebook"));
		EXPECT_EQ(QString("facebook"), facebook->name());
	}

	db.close();
}

} // namespace
