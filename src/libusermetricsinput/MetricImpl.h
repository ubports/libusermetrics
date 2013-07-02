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

#ifndef USERMETRICSINPUT_METRICIMPL_H_
#define USERMETRICSINPUT_METRICIMPL_H_

#include <libusermetricsinput/Metric.h>
#include <libusermetricscommon/UserMetricsInterface.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtDBus/QtDBus>

namespace UserMetricsInput {

class MetricImpl: public Metric {
public:
	explicit MetricImpl(const QString &dataSourceId,
			const QString &formatString, const QDBusConnection &dbusConnection,
			QObject *parent = 0);

	virtual ~MetricImpl();

	virtual MetricUpdate * update(const QString &username);

protected:
	QDBusConnection m_dbusConnection;

	com::canonical::UserMetrics m_interface;

	QString m_dataSourceId;

	QString m_formatString;
};

}

#endif // USERMETRICSINPUT_METRICIMPL_H_
