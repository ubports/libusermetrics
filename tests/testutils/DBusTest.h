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

#include <QtCore/QCoreApplication>
#include <QtCore/QProcess>
#include <QtDBus/QtDBus>

#include <gtest/gtest.h>

#ifndef USERMETRICS_TESTUTILS_TESTCOMMON_H_
#define USERMETRICS_TESTUTILS_TESTCOMMON_H_

namespace UserMetricsTestUtils {

class DBusTest: public ::testing::Test {
protected:
	DBusTest();

	virtual ~DBusTest();

	virtual void SetUp();

	virtual void TearDown();

	QScopedPointer<QDBusConnection> connection;

	QProcess dbus;

	QProcess userMetricsService;
};

}
#endif // USERMETRICS_TESTUTILS_TESTCOMMON_H_
