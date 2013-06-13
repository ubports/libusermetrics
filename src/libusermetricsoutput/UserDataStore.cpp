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

#include <libusermetricsoutput/UserDataStore.h>

using namespace UserMetricsOutput;

UserDataStore::UserDataStore(QObject *parent) :
		QObject(parent) {
//	DataSetPtr emptyData(new DataSet(ColorThemeImpl(), ColorThemeImpl(), this));
//	emptyData->setFormatString("No data sources available");
//	m_dataSets.insert("", emptyData);
}

UserDataStore::~UserDataStore() {
}

UserDataStore::const_iterator UserDataStore::constFind(
		const QString &username) const {
	return m_userData.constFind(username);
}

UserDataStore::const_iterator UserDataStore::constEnd() const {
	return m_userData.constEnd();
}

UserDataStore::iterator UserDataStore::find(const QString &username) {
	iterator data(m_userData.find(username));

	if (data == m_userData.end()) {
		UserDataPtr userData(new UserData());
		data = m_userData.insert(username, userData);
	}

	return data;
}
