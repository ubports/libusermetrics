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

#ifndef USERMETRICSSERVICE_DBUSUSERMETRICS_H_
#define USERMETRICSSERVICE_DBUSUSERMETRICS_H_

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QHash>

class UserMetricsAdaptor;

namespace UserMetricsCommon {
class DateFactory;
}

namespace UserMetricsService {

class DBusDataSource;
class DBusUserData;

class DBusUserMetrics: public QObject {
Q_OBJECT

Q_PROPERTY(QList<QDBusObjectPath> dataSources READ dataSources)

public:
	DBusUserMetrics(QDBusConnection &dbusConnection,
			QSharedPointer<UserMetricsCommon::DateFactory> dateFactory,
			QObject *parent = 0);

	virtual ~DBusUserMetrics();

	QList<QDBusObjectPath> dataSources() const;

	QDBusObjectPath createDataSource(const QString &name,
			const QString &formatString);

	QSharedPointer<DBusDataSource> dataSource(const QString &name) const;

	QList<QDBusObjectPath> userData() const;

	QDBusObjectPath createUserData(const QString &username);

	QSharedPointer<DBusUserData> userData(const QString &username) const;

public Q_SLOTS:

Q_SIGNALS:

protected:
	void syncDatabase();

	QDBusConnection m_dbusConnection;

	QScopedPointer<UserMetricsAdaptor> m_adaptor;

	QSharedPointer<UserMetricsCommon::DateFactory> m_dateFactory;

	QHash<int, QSharedPointer<DBusDataSource>> m_dataSources;

	QHash<int, QSharedPointer<DBusUserData>> m_userData;
};

}

#endif // USERMETRICSSERVICE_DBUSUSERMETRICS_H_
