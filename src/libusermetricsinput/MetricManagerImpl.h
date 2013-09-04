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

#ifndef USERMETRICSINPUT_METRICMANAGERIMPL_H_
#define USERMETRICSINPUT_METRICMANAGERIMPL_H_

#include <libusermetricsinput/MetricManager.h>
#include <libusermetricscommon/UserMetricsInterface.h>

#include <QtCore/QMap>

namespace UserMetricsInput {

class MetricManagerImpl: public MetricManager {
public:
	explicit MetricManagerImpl(const QDBusConnection &dbusConnection,
			QObject *parent = 0);

	virtual ~MetricManagerImpl();

	virtual MetricPtr add(const QString &dataSourceId,
			const QString &formatString, const QString &emptyDataString = "",
			const QString &textDomain = "");

	virtual MetricPtr add(const MetricParameters &parameters);

protected:
	QDBusConnection m_dbusConnection;

	com::canonical::UserMetrics m_interface;

	QMap<QString, MetricPtr> m_metrics;
};

}

#endif // USERMETRICSINPUT_METRICMANAGERIMPL_H_
