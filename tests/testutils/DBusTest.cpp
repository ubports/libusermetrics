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

#include <testutils/DBusTest.h>
#include <QtTest/QSignalSpy>
#include <libqtdbustest/QProcessDBusService.h>
#include <QtDBus/QtDBus>

using namespace UserMetricsTestUtils;
using namespace QtDBusTest;

DBusTest::DBusTest() {
	qputenv("USERMETRICS_NO_AUTH", "1");
	qputenv("USERMETRICS_NO_COLOR_SETTINGS", "1");
	qputenv("USERMETRICS_COLOR_SCHEMA_BASEDIR", USERMETRICS_TEST_COLOR_BASEDIR);
	qputenv("USERMETRICS_COLOR_THEME_BASEDIR", USERMETRICS_TEST_COLOR_BASEDIR);
	DBusServicePtr userMetricsService(
			new QProcessDBusService("com.canonical.UserMetrics",
					QDBusConnection::SystemBus, USERMETRICSSERVICE_BINARY,
					QStringList() << ":memory:"));
	dbus.registerService(userMetricsService);
}

DBusTest::~DBusTest() {
}

void DBusTest::SetUp() {
	dbus.startServices();
}

void DBusTest::TearDown() {
}

const QDBusConnection & DBusTest::systemConnection() const {
	return dbus.systemConnection();
}
