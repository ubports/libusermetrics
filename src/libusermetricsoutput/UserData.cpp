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

#include <libusermetricsoutput/UserData.h>
#include <libusermetricsoutput/UserMetricsStore.h>

using namespace UserMetricsOutput;

UserData::UserData(UserMetricsStore &userMetricsStore, QObject *parent) :
		QObject(parent), m_userMetricsStore(userMetricsStore) {
}

UserData::~UserData() {
}

UserData::const_iterator UserData::constBegin() const {
	return m_dataSets.constBegin();
}

UserData::const_iterator UserData::constEnd() const {
	return m_dataSets.constEnd();
}

UserData::iterator UserData::insert(const QString &dataSourceName,
		DataSetPtr dataSet) {
	auto it(m_dataSets.insert(dataSourceName, dataSet));

	dataSetAdded(dataSourceName);

	return it;
}

