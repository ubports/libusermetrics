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

#ifndef USERMETRICSOUTPUT_DATASETSTORE_H_
#define USERMETRICSOUTPUT_DATASETSTORE_H_

#include <libusermetricsoutput/UserData.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QMap>

namespace UserMetricsOutput {

class UserMetricsStore: public QObject {
public:
	typedef QSharedPointer<UserData> UserDataPtr;

	typedef QMap<QString, UserDataPtr> UserDataMap;

	typedef UserDataMap::iterator iterator;

	typedef UserDataMap::const_iterator const_iterator;

	UserMetricsStore(QObject *parent = 0);

	virtual ~UserMetricsStore();

	virtual const_iterator constFind(const QString &username) const;

	virtual const_iterator constEnd() const;

	virtual iterator find(const QString &username);

protected:
	UserDataMap m_userData;
};

}

#endif // USERMETRICSOUTPUT_DATASETSTORE_H_
