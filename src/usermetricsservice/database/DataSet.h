/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#ifndef USERMETRICSSERVICE_DATASET_H_
#define USERMETRICSSERVICE_DATASET_H_

#include <usermetricsservice/database/DataSource.h>
#include <usermetricsservice/database/UserData.h>

#include <QtCore/QDate>

#include <QDjangoModel.h>

namespace UserMetricsService {

class Q_DECL_EXPORT DataSet: public QDjangoModel {
Q_OBJECT

Q_PROPERTY(int id READ id WRITE setId)

Q_PROPERTY(UserMetricsService::UserData* userData READ userData WRITE setUserData)

Q_PROPERTY(UserMetricsService::DataSource* dataSource READ dataSource WRITE setDataSource)

Q_PROPERTY(QByteArray data READ data WRITE setData)

Q_PROPERTY(QDate lastUpdated READ lastUpdated WRITE setLastUpdated)

Q_CLASSINFO("id", "primary_key=true auto_increment=true")

Q_CLASSINFO("userData", "on_delete=cascade")

Q_CLASSINFO("dataSource", "on_delete=cascade")

Q_CLASSINFO("data", "null=true")

Q_CLASSINFO("lastUpdated", "null=true")

public:
	DataSet();

	virtual ~DataSet();

	static void findById(int id, DataSet *dataSet);

	int id() const;

	void setId(int id);

	UserData * userData() const;

	void setUserData(UserData *userData);

	DataSource * dataSource() const;

	void setDataSource(DataSource *dataSource);

	const QByteArray & data() const;

	void setData(const QByteArray &data);

	const QDate & lastUpdated() const;

	void setLastUpdated(const QDate &data);

protected:
	int m_id;

	QByteArray m_data;

	QDate m_lastUpdated;
};

}

#endif // USERMETRICSSERVICE_DATASET_H_
