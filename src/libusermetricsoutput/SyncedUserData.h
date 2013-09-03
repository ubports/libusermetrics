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

#ifndef USERMETRICSOUTPUT_SYNCEDUSERDATA_H_
#define USERMETRICSOUTPUT_SYNCEDUSERDATA_H_

#include <libusermetricsoutput/UserData.h>
#include <libusermetricscommon/UserDataInterface.h>

namespace UserMetricsOutput {

class UserMetricsStore;

class SyncedUserData: public UserData {
Q_OBJECT

public:
	explicit SyncedUserData(UserMetricsStore &userMetricsStore,
			QSharedPointer<com::canonical::usermetrics::UserData> interface,
			QObject *parent = 0);

	virtual ~SyncedUserData();

public Q_SLOTS:
	void addDataSet(const QString &dataSourceName, const QDBusObjectPath &path);

	void removeDataSet(const QString &dataSourceName,
			const QDBusObjectPath &path);

protected:
	QSharedPointer<com::canonical::usermetrics::UserData> m_interface;
};

}

#endif // USERMETRICSOUTPUT_SYNCEDUSERDATA_H_
