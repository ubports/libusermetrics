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

#include <libusermetricsinput/MetricImpl.h>
#include <libusermetricsinput/MetricUpdateImpl.h>
#include <libusermetricscommon/DBusPaths.h>
#include <libusermetricscommon/UserDataInterface.h>

#include <QtDBus/QtDBus>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsInput;

MetricImpl::MetricImpl(const std::string &dataSourceId,
		const std::string &formatString, const QDBusConnection &dbusConnection,
		QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_interface(
				DBusPaths::serviceName(), DBusPaths::userMetrics(),
				dbusConnection), m_dataSourceId(
				QString::fromStdString(dataSourceId)), m_formatString(
				QString::fromStdString(formatString)) {
}

MetricImpl::~MetricImpl() {
}

MetricUpdatePtr MetricImpl::update(const string &username) {
	QDBusObjectPath userDataPath(
			m_interface.createUserData(QString::fromStdString(username)));
	qDebug() << userDataPath.path();

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), userDataPath.path(), m_dbusConnection);
	Q_ASSERT(userDataInterface.isValid());

	QDBusObjectPath dataSetPath(
			userDataInterface.createDataSet(m_dataSourceId));
	qDebug() << dataSetPath.path();

	return MetricUpdatePtr(
			new MetricUpdateImpl(dataSetPath.path(), m_dbusConnection));
}
