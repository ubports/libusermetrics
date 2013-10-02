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
 */

#include <DBusQuery.h>
#include <MetricInfo.h>

#include <cmath>
#include <QtDBus/QtDBus>
#include <libqtdbustest/QProcessDBusService.h>
#include <libqtdbustest/DBusService.h>
#include <libusermetricscommon/UserMetricsInterface.h>
#include <libusermetricscommon/UserDataInterface.h>
#include <libusermetricscommon/DataSetInterface.h>
#include <libusermetricscommon/DataSourceInterface.h>
#include <libusermetricscommon/DBusPaths.h>

using namespace QtDBusTest;
using namespace UserMetricsCommon;

DBusQuery::DBusQuery(QObject *parent) :
		QObject(parent), dbus(0) {
	qputenv("USERMETRICS_NO_AUTH", "1");
	qputenv("USERMETRICS_NO_COLOR_SETTINGS", "1");
	qputenv("USERMETRICS_COLOR_SCHEMA_BASEDIR", USERMETRICS_TEST_COLOR_BASEDIR);
	qputenv("USERMETRICS_COLOR_THEME_BASEDIR", USERMETRICS_TEST_COLOR_BASEDIR);
	DBusServicePtr userMetricsService(
			new QProcessDBusService("com.canonical.UserMetrics",
					QDBusConnection::SystemBus, USERMETRICSSERVICE_BINARY,
					QStringList() << ":memory:"));
	dbus.registerService(userMetricsService);
	dbus.startServices();
}

double DBusQuery::queryCurrentValue(int index) {
	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), DBusPaths::dataSet(index),
			dbus.systemConnection());
	QVariantList data = dataSetInterface.data();
	return data.at(0).toDouble();
}

MetricInfo* DBusQuery::queryMetricInfo(int index) {
	com::canonical::usermetrics::DataSource dataSourceInterface(
			DBusPaths::serviceName(), DBusPaths::dataSource(index),
			dbus.systemConnection());
	if (dataSourceInterface.name().isEmpty()) {
		return 0;
	} else {
		double minimum(NAN);
		double maximum(NAN);
		QVariantMap options(dataSourceInterface.options());
		if (options.contains("minimum")) {
			minimum = options["minimum"].toDouble();
		}
		if (options.contains("maximum")) {
			maximum = options["maximum"].toDouble();
		}

		return new MetricInfo(dataSourceInterface.name(),
				dataSourceInterface.formatString(),
				dataSourceInterface.emptyDataString(),
				dataSourceInterface.textDomain(), minimum, maximum, this);
	}
}

