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

#ifndef USERMETRICSOUTPUT_SYNCEDUSERMETRICSSTORE_H_
#define USERMETRICSOUTPUT_SYNCEDUSERMETRICSSTORE_H_

#include <libusermetricsoutput/UserMetricsStore.h>
#include <libusermetricscommon/UserMetricsInterface.h>

namespace UserMetricsOutput {

class SyncedUserMetricsStore: public UserMetricsStore {
public:
	SyncedUserMetricsStore(const QDBusConnection &dbusConnection,
			QObject *parent = 0);

	virtual ~SyncedUserMetricsStore();

protected:
	QDBusConnection m_dbusConnection;

	com::canonical::UserMetrics m_interface;
};

}

#endif // USERMETRICSOUTPUT_SYNCEDUSERMETRICSSTORE_H_
