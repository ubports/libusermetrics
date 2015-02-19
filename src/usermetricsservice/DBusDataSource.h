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
#include <QtCore/QSharedPointer>
#include <QtDBus/QDBusContext>
#include <QtDBus/QDBusConnection>

class DataSourceAdaptor;

namespace UserMetricsService {

class DataSource;
class DBusDataSource;
class TranslationLocator;

typedef QSharedPointer<DBusDataSource> DBusDataSourcePtr;

class DBusDataSource: public QObject, protected QDBusContext {
Q_OBJECT

Q_PROPERTY(QString name READ name)

Q_PROPERTY(QString formatString READ formatString WRITE setFormatString)

Q_PROPERTY(QString emptyDataString READ emptyDataString WRITE setEmptyDataString)

Q_PROPERTY(QString textDomain READ textDomain WRITE setTextDomain)

Q_PROPERTY(QString translationPath READ translationPath)

Q_PROPERTY(unsigned int metricType READ metricType WRITE setMetricType)

Q_PROPERTY(QVariantMap options READ options)

public:
	DBusDataSource(int id, const QString &name, const QString &packageId,
			QDBusConnection &dbusConnection, QSharedPointer<TranslationLocator>,
			QObject *parent = 0);

	virtual ~DBusDataSource();

	QString path() const;

	QString translationPath() const;

	QString name() const;

	QString formatString() const;

	void setFormatString(const QString &formatString);

	QString emptyDataString() const;

	void setEmptyDataString(const QString &emptyDataString);

	QString textDomain() const;

	void setTextDomain(const QString &textDomain);

	void setSecret(const QString &secret);

	unsigned int metricType() const;

	void setMetricType(unsigned int type);

	bool hasMinimum() const;

	void noMinimum();

	double minimum() const;

	void setMinimum(double minimum);

	bool hasMaximum() const;

	void noMaximum();

	double maximum() const;

	void setMaximum(double maximum);

	QVariantMap options() const;

protected:
	void lookupDataSource(DataSource *dataSource) const;

	QVariantMap generateOptions(const DataSource &dataSource) const;

	QDBusConnection m_dbusConnection;

	QScopedPointer<DataSourceAdaptor> m_adaptor;

	int m_id;

	QString m_path;

	QString m_translationPath;

	QString m_name;

	QString m_packageId;

	QSharedPointer<TranslationLocator> m_translationLocator;
};

}

#endif // USERMETRICSSERVICE_DBUSDATASOURCE_H_
