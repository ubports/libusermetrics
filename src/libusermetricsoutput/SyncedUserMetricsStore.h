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
#include <libusermetricscommon/UserDataInterface.h>

namespace UserMetricsOutput {

class SyncedUserMetricsStore: public UserMetricsStore {
Q_OBJECT

public:
	explicit SyncedUserMetricsStore(const QDBusConnection &dbusConnection,
			QObject *parent = 0);

	virtual ~SyncedUserMetricsStore();

Q_SIGNALS:
	void connectionEstablished();

public Q_SLOTS:
	void addUserData(const QString &username, const QDBusObjectPath &path);

	void removeUserData(const QString &username, const QDBusObjectPath &path);

	void addDataSource(const QDBusObjectPath &path);

	void removeDataSource(const QDBusObjectPath &path);

	void sync();

protected:
	void attachSystemData(
			QSharedPointer<com::canonical::usermetrics::UserData> systemData);

	com::canonical::UserMetrics m_interface;
}
;

}

#endif // USERMETRICSOUTPUT_SYNCEDUSERMETRICSSTORE_H_
