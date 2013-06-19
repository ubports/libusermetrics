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

#include <QtCore/QObject>

namespace UserMetricsInput {

class MetricImpl: public Metric, public QObject {
public:
	explicit MetricImpl(QObject *parent = 0);

	virtual ~MetricImpl();

	virtual MetricUpdatePtr update(const std::string &username);
};

}

#endif // USERMETRICSINPUT_METRICIMPL_H_
