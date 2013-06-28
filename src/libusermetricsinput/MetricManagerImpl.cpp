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
#include <libusermetricsinput/MetricManagerImpl.h>
#include <libusermetricscommon/DBusPaths.h>

#include <QtDBus/QtDBus>
#include <QtCore/QDebug>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsInput;

MetricManagerImpl::MetricManagerImpl(const QDBusConnection &dbusConnection,
		QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_interface(
				DBusPaths::serviceName(), DBusPaths::userMetrics(),
				dbusConnection) {
}

MetricManagerImpl::~MetricManagerImpl() {
}

MetricPtr MetricManagerImpl::add(const string &dataSourceIdIn,
		const string &formatStringIn) {
	QString dataSourceId(QString::fromStdString(dataSourceIdIn));
	QString formatString(QString::fromStdString(formatStringIn));

	QDBusObjectPath path(
			m_interface.createDataSource(dataSourceId, formatString));

	auto metric(m_metrics.find(dataSourceId));
	if (metric == m_metrics.end()) {
		MetricPtr newMetric(
				new MetricImpl(dataSourceIdIn, formatStringIn,
						m_dbusConnection));
		metric = m_metrics.insert(dataSourceId, newMetric);
	}
	return *metric;
}
