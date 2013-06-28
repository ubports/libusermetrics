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

#include <libusermetricsinput/MetricUpdateImpl.h>
#include <libusermetricscommon/DBusPaths.h>

using namespace UserMetricsCommon;
using namespace UserMetricsInput;

MetricUpdateImpl::MetricUpdateImpl(const QString &path,
		const QDBusConnection &dbusConnection, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_interface(
				DBusPaths::serviceName(), path, dbusConnection) {
}

MetricUpdateImpl::~MetricUpdateImpl() {
	QDBusPendingReply<void> reply(m_interface.update(m_data));
	reply.waitForFinished();
}

void MetricUpdateImpl::addData(double data) {
	m_data << data;
}

void MetricUpdateImpl::addNull() {
//	m_data << QVariant();
}
