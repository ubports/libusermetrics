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

#ifndef USERMETRICSSERVICE_DBUSDATASOURCE_H_
#define USERMETRICSSERVICE_DBUSDATASOURCE_H_

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

#include <QDBusConnection>

class DataSourceAdaptor;

namespace UserMetricsService {

class DataSource;
class DBusDataSource;

typedef QSharedPointer<DBusDataSource> DBusDataSourcePtr;

class DBusDataSource: public QObject {
Q_OBJECT

Q_PROPERTY(QString name READ name)

Q_PROPERTY(QString formatString READ formatString WRITE setFormatString)

public:
	DBusDataSource(int id, QDBusConnection &dbusConnection,
			QObject *parent = 0);

	virtual ~DBusDataSource();

	QString path() const;

	QString name() const;

	QString formatString() const;

	void setFormatString(const QString &formatString);

protected:
	void lookupDataSource(DataSource *dataSource) const;

	QDBusConnection m_dbusConnection;

	QScopedPointer<DataSourceAdaptor> m_adaptor;

	int m_id;

	QString m_path;
};

}

#endif // USERMETRICSSERVICE_DBUSDATASOURCE_H_
