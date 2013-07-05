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
#include <cstdlib>

using namespace UserMetricsTestUtils;

DBusTest::DBusTest() {
}

DBusTest::~DBusTest() {
}

void DBusTest::SetUp() {
	dbus.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params << "--config-file" << DBUS_SYSTEM_CONFIG_FILE << "--print-address";
	dbus.start("dbus-daemon", params);
	EXPECT_TRUE(dbus.waitForStarted());

	dbus.waitForReadyRead();
	QByteArray readAll(dbus.readAll());
	bus = readAll.trimmed();

	QByteArray byteArray(bus.toUtf8());
	setenv("DBUS_SYSTEM_BUS_ADDRESS", byteArray.data(), true);

	connection.reset(
			new QDBusConnection(QDBusConnection::connectToBus(bus, bus)));

	QDBusServiceWatcher watcher("com.canonical.UserMetrics", *connection,
			QDBusServiceWatcher::WatchForRegistration);
	QSignalSpy spy(&watcher,
			SIGNAL(serviceOwnerChanged(const QString &,const QString &,const QString &)));

	QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
	env.insert("DBUS_SYSTEM_BUS_ADDRESS", bus);
	env.insert("DBUS_SESSION_BUS_ADDRESS", bus);
	userMetricsService.setProcessChannelMode(QProcess::MergedChannels);
	userMetricsService.setProcessEnvironment(env);
	userMetricsService.start(USERMETRICSSERVICE_BINARY,
			QStringList() << ":memory:");

	spy.wait();
	QVariantList arguments(spy.takeFirst());
	EXPECT_EQ("com.canonical.UserMetrics", arguments.first().toString());
}

void DBusTest::TearDown() {
	userMetricsService.waitForReadyRead(100);
	qDebug() << userMetricsService.readAll();

	userMetricsService.terminate();
	EXPECT_TRUE(userMetricsService.waitForFinished());

	dbus.terminate();
	EXPECT_TRUE(dbus.waitForFinished());
}
