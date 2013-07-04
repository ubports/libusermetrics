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

#include <stdexcept>

#include <libusermetricsinput/MetricImpl.h>
#include <libusermetricsinput/MetricUpdateImpl.h>
#include <libusermetricscommon/DBusPaths.h>
#include <libusermetricscommon/UserDataInterface.h>

#include <QtDBus/QtDBus>

#include <cstdlib>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsInput;

MetricImpl::MetricImpl(const QString &dataSourceId, const QString &formatString,
		const QDBusConnection &dbusConnection, QObject *parent) :
		Metric(parent), m_dbusConnection(dbusConnection), m_interface(
				DBusPaths::serviceName(), DBusPaths::userMetrics(),
				dbusConnection), m_dataSourceId(dataSourceId), m_formatString(
				formatString) {
}

MetricImpl::~MetricImpl() {
}

MetricUpdate * MetricImpl::update(const QString &usernameIn) {
	QString username;
	if (usernameIn.isEmpty()) {
		username = QString::fromUtf8(getenv("USER"));
	} else {
		username = usernameIn;
	}

	QDBusObjectPath userDataPath(m_interface.createUserData(username));

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), userDataPath.path(), m_dbusConnection);
	if (!userDataInterface.isValid()) {
		throw logic_error("user data interface invalid");
	}

	QDBusObjectPath dataSetPath(
			userDataInterface.createDataSet(m_dataSourceId));

	return new MetricUpdateImpl(dataSetPath.path(), m_dbusConnection);
}
