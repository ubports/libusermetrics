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
#include <QtCore/QSharedPointer>
#include <QtDBus/QDBusContext>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusObjectPath>

class DataSetAdaptor;

namespace UserMetricsCommon {
class DateFactory;
}

namespace UserMetricsService {

class Authentication;
class DataSet;
class DBusDataSet;

typedef QSharedPointer<DBusDataSet> DBusDataSetPtr;

class DBusDataSet: public QObject, protected QDBusContext {
Q_OBJECT

Q_PROPERTY(QVariantList data READ data)

Q_PROPERTY(uint lastUpdated READ lastUpdated)

Q_PROPERTY(QDBusObjectPath dataSource READ dataSource)

public:
	DBusDataSet(int id, const QString &dataSource,
			QDBusConnection &dbusConnection,
			QSharedPointer<UserMetricsCommon::DateFactory> dateFactory,
			QSharedPointer<Authentication> authentication, QObject *parent = 0);

	virtual ~DBusDataSet();

	QString path() const;

	QDBusObjectPath dataSource() const;

	int id() const;

	QVariantList data() const;

	uint lastUpdated() const;

	QDate lastUpdatedDate() const;

public Q_SLOTS:
	void update(const QVariantList &data);

	void increment(double amount);

protected:
	static void getData(DataSet &dataSet, QVariantList &data);

	void internalUpdate(DataSet &dataSet, const QVariantList &oldData,
			const QVariantList &data);

	QDBusConnection m_dbusConnection;

	QScopedPointer<DataSetAdaptor> m_adaptor;

	QSharedPointer<UserMetricsCommon::DateFactory> m_dateFactory;

	QSharedPointer<Authentication> m_authentication;

	int m_id;

	QString m_path;

	QString m_dataSource;
};

}

#endif // USERMETRICSSERVICE_DBUSDATASET_H_
