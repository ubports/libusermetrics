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

#ifndef USERMETRICSINPUT_METRICMANAGER_H_
#define USERMETRICSINPUT_METRICMANAGER_H_

#include <libusermetricsinput/Metric.h>

namespace UserMetricsInput {

class MetricManager;

typedef QScopedPointer<MetricManager> MetricManagerPtr;

class Q_DECL_EXPORT MetricManager: public QObject {
public:
	explicit MetricManager(QObject *parent = 0);

	virtual ~MetricManager();

	static MetricManager * getInstance();

	virtual MetricPtr add(const QString &dataSourceId,
			const QString &formatString) = 0;
};

}
#endif // USERMETRICSINPUT_METRICMANAGER_H_
