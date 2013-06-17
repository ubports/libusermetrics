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

#ifndef USERMETRICSINPUT_METRICUPDATE_H_
#define USERMETRICSINPUT_METRICUPDATE_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>

namespace UserMetricsInput {

class MetricUpdate;

typedef QSharedPointer<MetricUpdate> MetricUpdatePtr;

class Q_DECL_EXPORT MetricUpdate: public QObject {
public:
	virtual ~MetricUpdate();

	static MetricUpdatePtr getInstance(const QString &dataSourceId,
			const QString &username);

	virtual void addData(float data) = 0;

	virtual void addNull() = 0;

protected:
	MetricUpdate(QObject *parent = 0);
};

}

#endif // USERMETRICSINPUT_METRICUPDATE_H_
