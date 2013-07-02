/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <libusermetricsinput/MetricManagerImpl.h>
#include <libusermetricscommon/DBusPaths.h>
#include <QtDBus/QtDBus>

using namespace UserMetricsCommon;
using namespace UserMetricsInput;

MetricManager::MetricManager(QObject *parent) :
		QObject(parent) {

}

MetricManager::~MetricManager() {
}

MetricManager * MetricManager::getInstance() {
	QDBusConnection dbusConnection(QDBusConnection::systemBus());

	QDBusConnectionInterface* interface = dbusConnection.interface();
	if (!interface->isServiceRegistered(DBusPaths::serviceName())) {
		QDBusReply<void> reply(
				interface->startService(DBusPaths::serviceName()));
	}

	return new MetricManagerImpl(dbusConnection);
}
