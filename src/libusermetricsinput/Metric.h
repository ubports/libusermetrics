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

#ifndef USERMETRICSINPUT_METRIC_H_
#define USERMETRICSINPUT_METRIC_H_

#include <libusermetricsinput/MetricUpdate.h>
#include <QtCore/QSharedPointer>

namespace UserMetricsInput {

class Metric;

typedef QSharedPointer<Metric> MetricPtr;

class Q_DECL_EXPORT Metric: public QObject {
public:
	explicit Metric(QObject *parent = 0);

	virtual ~Metric();

	virtual MetricUpdate * update(const QString &username) = 0;

};

}

#endif // USERMETRICSINPUT_METRIC_H_
