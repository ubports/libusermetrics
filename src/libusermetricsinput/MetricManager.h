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

/*@{*/

namespace UserMetricsInput {

class MetricManager;

typedef QScopedPointer<MetricManager> MetricManagerPtr;

/** @brief Central place for registering and updating user metrics.
 *
 * More details.
 **/
class Q_DECL_EXPORT MetricManager: public QObject {
public:
	/**
	 * @brief Foo
	 */
	explicit MetricManager(QObject *parent = 0);

	/**
	 * @brief Foo
	 */
	virtual ~MetricManager();

	/**
	 * @brief Foo
	 */
	static MetricManager * getInstance();

	/**
	 * @brief Foo
	 */
	virtual MetricPtr add(const QString &dataSourceId,
			const QString &formatString, const QString &textDomain = "") = 0;
};

/**
 * @example MetricManagerSingleDay.cpp
 *   UserMetricsInput::MetricManager example
 * @example MetricManagerAdvanced.cpp
 *   UserMetricsInput::MetricManager example 2
 **/
}


/*@}*/

#endif // USERMETRICSINPUT_METRICMANAGER_H_
