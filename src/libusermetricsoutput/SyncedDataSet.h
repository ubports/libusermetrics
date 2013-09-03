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

#ifndef USERMETRICSOUTPUT_SYNCEDDATASET_H_
#define USERMETRICSOUTPUT_SYNCEDDATASET_H_

#include <libusermetricsoutput/DataSet.h>
#include <libusermetricscommon/DataSetInterface.h>

namespace UserMetricsOutput {

class SyncedDataSet: public DataSet {
Q_OBJECT

public:
	explicit SyncedDataSet(
			QSharedPointer<com::canonical::usermetrics::DataSet> interface,
			DataSourcePtr dataSource, QObject *parent = 0);

	virtual ~SyncedDataSet();

protected:
	QSharedPointer<com::canonical::usermetrics::DataSet> m_interface;
};

}

#endif // USERMETRICSOUTPUT_SYNCEDDATASET_H_
