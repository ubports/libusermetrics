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

#include "dbusquery.h"

#include <QDebug>
#include <QtDBus/QtDBus>
#include <libqtdbustest/QProcessDBusService.h>
#include <libqtdbustest/DBusService.h>
#include <libusermetricscommon/UserMetricsInterface.h>
#include <libusermetricscommon/DBusPaths.h>

using namespace QtDBusTest;
using namespace UserMetricsCommon;

DBusQuery::DBusQuery(QObject *parent) :
    QObject(parent)
{
    DBusServicePtr userMetricsService(
            new QProcessDBusService("com.canonical.UserMetrics",
                    QDBusConnection::SystemBus, USERMETRICSSERVICE_BINARY,
                    QStringList() << ":memory:"));
    dbus.registerService(userMetricsService);
}

void DBusQuery::start() {
    dbus.startServices();
}

float DBusQuery::queryValue() {
    com::canonical::UserMetrics userMetricsInterface(DBusPaths::serviceName(),
            DBusPaths::userMetrics(), dbus.systemConnection());

    return 0.0f;
}

