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

#ifndef SYNCEDDATASOURCE_H_
#define SYNCEDDATASOURCE_H_

#include <libusermetricsoutput/DataSource.h>
#include <libusermetricscommon/DataSourceInterface.h>

namespace UserMetricsOutput {

class SyncedDataSource: public DataSource {
Q_OBJECT

public:
	SyncedDataSource(
			QSharedPointer<com::canonical::usermetrics::DataSource> interface,
			QObject *parent = 0);

	virtual ~SyncedDataSource();

protected Q_SLOTS:
	void setTypeUint(uint type);

protected:
	QSharedPointer<com::canonical::usermetrics::DataSource> m_interface;
};

}

#endif /* SYNCEDDATASOURCE_H_ */
