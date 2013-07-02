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

#ifndef USERMETRICSINPUT_METRICUPDATEIMPL_H_
#define USERMETRICSINPUT_METRICUPDATEIMPL_H_

#include <QtCore/QObject>
#include <QtCore/QVariantList>
#include <QtDBus/QtDBus>

#include <libusermetricsinput/MetricUpdate.h>
#include <libusermetricscommon/DataSetInterface.h>

namespace UserMetricsInput {

class Metric;

class MetricUpdateImpl: public MetricUpdate {
public:
	explicit MetricUpdateImpl(const QString &path,
			const QDBusConnection &dbusConnection, QObject *parent = 0);

	virtual ~MetricUpdateImpl();

	virtual void addData(double data);

	virtual void addNull();

protected:
	QDBusConnection m_dbusConnection;

	com::canonical::usermetrics::DataSet m_interface;

	QVariantList m_data;
};

}

#endif // USERMETRICSINPUT_METRICUPDATEIMPL_H_
