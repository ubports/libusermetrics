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

#include <memory>

#include <libusermetricsinput/UserMetricsInputExport.h>
#include <libusermetricsinput/MetricUpdate.h>

namespace UserMetricsInput {

class Metric;

typedef std::unique_ptr<Metric> MetricPtr;

class USERMETRICSINPUT_EXPORT Metric {
public:
	virtual ~Metric();

	virtual MetricUpdatePtr update(const std::string &username) = 0;

protected:
	explicit Metric();
};

}

#endif // USERMETRICSINPUT_METRIC_H_
