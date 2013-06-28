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

#ifndef USERMETRICSSERVICE_DBUSDATASET_H_
#define USERMETRICSSERVICE_DBUSDATASET_H_

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QScopedPointer>
#include <QtDBus/QtDBus>

class DataSetAdaptor;

namespace UserMetricsCommon {
class DateFactory;
}

namespace UserMetricsService {

class DataSet;
class DBusDataSet;

typedef QSharedPointer<DBusDataSet> DBusDataSetPtr;

class DBusDataSet: public QObject {
Q_OBJECT

Q_PROPERTY(QVariantList data READ data)

Q_PROPERTY(uint lastUpdated READ lastUpdated)

Q_PROPERTY(QString dataSource READ dataSource)

public:
	DBusDataSet(int id, QDBusConnection &dbusConnection,
			QSharedPointer<UserMetricsCommon::DateFactory> dateFactory,
			QObject *parent = 0);

	virtual ~DBusDataSet();

	QString path() const;

	QString dataSource() const;

	int id() const;

	QVariantList data() const;

	void update(const QVariantList &data);

	uint lastUpdated() const;

	QDate lastUpdatedDate() const;

protected:
	static void getData(DataSet &dataSet, QVariantList &data);

	QDBusConnection m_dbusConnection;

	QScopedPointer<DataSetAdaptor> m_adaptor;

	QSharedPointer<UserMetricsCommon::DateFactory> m_dateFactory;

	int m_id;

	QString m_path;
};

}

#endif // USERMETRICSSERVICE_DBUSDATASET_H_
