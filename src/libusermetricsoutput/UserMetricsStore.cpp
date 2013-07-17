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

#include <libusermetricsoutput/UserMetricsStore.h>

using namespace UserMetricsOutput;

UserMetricsStore::UserMetricsStore(QObject *parent) :
		QObject(parent) {
}

UserMetricsStore::~UserMetricsStore() {
}

UserMetricsStore::const_iterator UserMetricsStore::constFind(
		const QString &username) const {
	return m_userData.constFind(username);
}

UserMetricsStore::const_iterator UserMetricsStore::constEnd() const {
	return m_userData.constEnd();
}

UserMetricsStore::iterator UserMetricsStore::insert(const QString &username,
		UserDataPtr userData) {
	auto it(m_userData.insert(username, userData));
	userDataAdded(username, userData);
	return it;
}

DataSourcePtr UserMetricsStore::dataSource(const QString &name) {
	return m_dataSources.value(name);
}

void UserMetricsStore::insert(const QString &name, DataSourcePtr dataSource) {
	m_dataSources.insert(name, dataSource);
}
