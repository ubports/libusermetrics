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
#include <libusermetricscommon/DataSetInterface.h>

#include <QtDBus/QtDBus>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsInput;

MetricImpl::MetricImpl(const QString &dataSourceId, const QString &formatString,
		const QString &dataSourcePath, const QDBusConnection &dbusConnection,
		QObject *parent) :
		Metric(parent), m_dbusConnection(dbusConnection), m_userMetrics(
				DBusPaths::serviceName(), DBusPaths::userMetrics(),
				dbusConnection), m_dataSource(DBusPaths::serviceName(),
				dataSourcePath, dbusConnection), m_dataSourceId(dataSourceId), m_formatString(
				formatString) {
}

MetricImpl::~MetricImpl() {
}

QDBusObjectPath MetricImpl::createDataSet(const QString &usernameIn) {
	QString username;
	if (m_dataSource.metricType() == MetricType::SYSTEM) {
		username = "";
	} else if (usernameIn.isEmpty()) {
		username = QString::fromUtf8(qgetenv("USER"));
	} else {
		username = usernameIn;
	}

	QDBusPendingReply<QDBusObjectPath> userDataReply(
			m_userMetrics.createUserData(username));

	userDataReply.waitForFinished();
	if (userDataReply.isError()) {
		throw logic_error(userDataReply.error().message().toStdString());
	}

	QDBusObjectPath userDataPath(userDataReply);

	com::canonical::usermetrics::UserData userDataInterface(
			DBusPaths::serviceName(), userDataPath.path(), m_dbusConnection);
	if (!userDataInterface.isValid()) {
		throw logic_error("user data interface invalid");
	}

	QDBusPendingReply<QDBusObjectPath> dataSetReply(
			userDataInterface.createDataSet(m_dataSourceId));

	dataSetReply.waitForFinished();
	if (dataSetReply.isError()) {
		throw logic_error(dataSetReply.error().message().toStdString());
	}

	return dataSetReply;
}

MetricUpdate * MetricImpl::update(const QString &username) {
	QDBusObjectPath dataSetPath(createDataSet(username));

	return new MetricUpdateImpl(dataSetPath.path(), m_dbusConnection);
}

void MetricImpl::update(double value, const QString &username) {
	QDBusObjectPath dataSetPath(createDataSet(username));

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), dataSetPath.path(), m_dbusConnection);

	QDBusPendingReply<> reply(dataSetInterface.update(QVariantList() << value));

	reply.waitForFinished();
	if (reply.isError()) {
		throw logic_error(reply.error().message().toStdString());
	}
}

void MetricImpl::increment(double amount, const QString &username) {
	QDBusObjectPath dataSetPath(createDataSet(username));

	com::canonical::usermetrics::DataSet dataSetInterface(
			DBusPaths::serviceName(), dataSetPath.path(), m_dbusConnection);

	QDBusPendingReply<> reply(dataSetInterface.increment(amount));

	reply.waitForFinished();
	if (reply.isError()) {
		throw logic_error(reply.error().message().toStdString());
	}
}
