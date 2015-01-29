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

#include <stdexcept>

#include <usermetricsservice/Authentication.h>
#include <usermetricsservice/DBusDataSource.h>
#include <usermetricsservice/DBusUserMetrics.h>
#include <usermetricsservice/DBusUserData.h>
#include <usermetricsservice/UserMetricsAdaptor.h>
#include <usermetricsservice/database/DataSource.h>
#include <usermetricsservice/database/UserData.h>
#include <usermetricsservice/database/DataSet.h>
#include <libusermetricscommon/DateFactory.h>
#include <libusermetricscommon/DBusPaths.h>
#include <libusermetricscommon/Localisation.h>

#include <QDjango.h>
#include <QDjangoQuerySet.h>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

DBusUserMetrics::DBusUserMetrics(const QDBusConnection &dbusConnection,
		QSharedPointer<DateFactory> dateFactory,
		QSharedPointer<Authentication> authentication,
		QSharedPointer<TranslationLocator> translationLocator, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new UserMetricsAdaptor(this)), m_dateFactory(dateFactory), m_authentication(
				authentication), m_translationLocator(translationLocator) {
	// Database setup
	QDjango::registerModel<DataSource>().createTable();
	QDjango::registerModel<UserData>().createTable();
	QDjango::registerModel<DataSet>().createTable();

	// DBus setup

	if (!m_dbusConnection.registerObject(DBusPaths::userMetrics(), this)) {
		throw logic_error(_("Unable to register user metrics object on DBus"));
	}

	syncDatabase();
}

DBusUserMetrics::~DBusUserMetrics() {
	m_dbusConnection.unregisterObject(DBusPaths::userMetrics());
}

QList<QDBusObjectPath> DBusUserMetrics::dataSources() const {
	QList<QDBusObjectPath> dataSources;
	for (DBusDataSourcePtr dataSource : m_dataSources.values()) {
		dataSources << QDBusObjectPath(dataSource->path());
	}
	return dataSources;
}

void DBusUserMetrics::syncDatabase() {
	{
		QSet<int> dataSourceNames;
		QDjangoQuerySet<DataSource> query;
		for (const DataSource &dataSource : query) {
			const int id(dataSource.id());
			dataSourceNames << id;
			// if we don't have a local cache
			if (!m_dataSources.contains(id)) {
				DBusDataSourcePtr dbusDataSource(
						new DBusDataSource(id, dataSource.name(), dataSource.secret(),
								m_dbusConnection, m_translationLocator));
				m_dataSources.insert(id, dbusDataSource);
				m_adaptor->dataSourceAdded(
						QDBusObjectPath(dbusDataSource->path()));
			}
		}
		// remove any cached references to deleted sources
		QSet<int> cachedDataSourceNames(
				QSet<int>::fromList(m_dataSources.keys()));
		QSet<int> &toRemove(cachedDataSourceNames.subtract(dataSourceNames));
		for (int id : toRemove) {
			DBusDataSourcePtr dataSource(m_dataSources.take(id));
			m_adaptor->dataSourceRemoved(QDBusObjectPath(dataSource->path()));
		}
	}

	{
		QSet<int> usernames;
		QDjangoQuerySet<UserData> query;
		for (const UserData &userData : query) {
			const int id(userData.id());
			usernames << id;
			// if we don't have a local cache
			if (!m_userData.contains(id)) {
				DBusUserDataPtr dbusUserData(
						new DBusUserData(id, userData.username(), *this,
								m_dbusConnection, m_dateFactory,
								m_authentication));
				m_userData.insert(id, dbusUserData);
				m_adaptor->userDataAdded(dbusUserData->username(),
						QDBusObjectPath(dbusUserData->path()));
			}
		}
		// remove any cached references to deleted sources
		QSet<int> cachedUsernames(QSet<int>::fromList(m_userData.keys()));
		QSet<int> &toRemove(cachedUsernames.subtract(usernames));
		for (int id : toRemove) {
			DBusUserDataPtr userData(m_userData.take(id));
			m_adaptor->userDataRemoved(userData->username(),
					QDBusObjectPath(userData->path()));
		}
	}
}

QDBusObjectPath DBusUserMetrics::createDataSource(const QString &name,
		const QString &formatString, const QString &emptyDataString,
		const QString &textDomain, int type, const QVariantMap &options) {

	QString confinementContext(m_authentication->getConfinementContext(*this));

	QDjangoQuerySet<DataSource> dataSourcesQuery;
	QDjangoQuerySet<DataSource> query(
			dataSourcesQuery.filter(
					QDjangoWhere("name", QDjangoWhere::Equals, name)
							&& QDjangoWhere("secret", QDjangoWhere::Equals,
									confinementContext)));

	if (query.size() == -1) {
		throw logic_error(_("Data source query failed"));
	}

	// If there is both an unconfined one and a confined one
	if (query.size() == 0) {
		query = dataSourcesQuery.filter(
				QDjangoWhere("name", QDjangoWhere::Equals, name)
						&& QDjangoWhere("secret", QDjangoWhere::Equals,
								"unconfined"));
	}

	if (query.size() == -1) {
		throw logic_error(_("Data source query failed"));
	}

	DataSource dataSource;

	bool hasMinimum(options.contains("minimum"));
	bool hasMaximum(options.contains("maximum"));

	double minimum(0);
	if (hasMinimum) {
		minimum = options["minimum"].toDouble();
	}
	double maximum(0);
	if (hasMaximum) {
		maximum = options["maximum"].toDouble();
	}

	if (query.size() == 0) {
		dataSource.setName(name);
		dataSource.setFormatString(formatString);
		dataSource.setEmptyDataString(emptyDataString);
		dataSource.setTextDomain(textDomain);
		dataSource.setType(type);
		dataSource.setSecret(confinementContext);
		if (hasMinimum) {
			dataSource.setMinimum(minimum);
			dataSource.setHasMinimum(true);
		}
		if (hasMaximum) {
			dataSource.setMaximum(maximum);
			dataSource.setHasMaximum(true);
		}

		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}

		syncDatabase();
	} else {
		query.at(0, &dataSource);
		const DBusDataSourcePtr dbusDataSource(
				*m_dataSources.constFind(dataSource.id()));

		if (dataSource.secret() == "unconfined") {
			if (confinementContext != "unconfined") {
				dbusDataSource->setSecret(confinementContext);
			}
		}

		if (dataSource.formatString() != formatString) {
			dbusDataSource->setFormatString(formatString);
		}
		if (dataSource.emptyDataString() != emptyDataString) {
			dbusDataSource->setEmptyDataString(emptyDataString);
		}
		if (dataSource.textDomain() != textDomain) {
			dbusDataSource->setTextDomain(textDomain);
		}
		if (dataSource.type() != type) {
			dbusDataSource->setMetricType(type);
		}
		if (dataSource.hasMinimum() != hasMinimum
				|| dataSource.minimum() != minimum) {
			if (hasMinimum) {
				dbusDataSource->setMinimum(minimum);
			} else {
				dbusDataSource->noMinimum();
			}
		}
		if (dataSource.hasMaximum() != hasMaximum
				|| dataSource.maximum() != maximum) {
			if (hasMaximum) {
				dbusDataSource->setMaximum(maximum);
			} else {
				dbusDataSource->noMaximum();
			}
		}
	}

	return QDBusObjectPath((*m_dataSources.constFind(dataSource.id()))->path());
}

QList<QDBusObjectPath> DBusUserMetrics::userDatas() const {
	QList<QDBusObjectPath> userDatas;
	for (DBusUserDataPtr userData : m_userData.values()) {
		userDatas << QDBusObjectPath(userData->path());
	}
	return userDatas;
}

QDBusObjectPath DBusUserMetrics::createUserData(const QString &username) {
	QString dbusUsername(m_authentication->getUsername(*this));

	if (!dbusUsername.isEmpty() && !username.isEmpty()
			&& dbusUsername != username) {
		m_authentication->sendErrorReply(*this, QDBusError::AccessDenied,
				_("Attempt to create user data owned by another user"));
		return QDBusObjectPath();
	}

	QDjangoQuerySet<UserData> query(
			QDjangoQuerySet<UserData>().filter(
					QDjangoWhere("username", QDjangoWhere::Equals, username)));

	if (query.size() == -1) {
		throw logic_error(_("User data query failed"));
	}

	UserData userData;

	if (query.size() == 0) {
		userData.setUsername(username);

		if (!userData.save()) {
			throw logic_error(_("Could not save user data"));
		}

		syncDatabase();
	} else {
		query.at(0, &userData);
	}

	return QDBusObjectPath((*m_userData.constFind(userData.id()))->path());
}

DBusDataSourcePtr DBusUserMetrics::dataSource(const QString &name,
		const QString &secret) const {
	DataSource dataSource;
	DataSource::findByNameAndSecret(name, secret, &dataSource);

	return m_dataSources.value(dataSource.id());
}

DBusUserDataPtr DBusUserMetrics::userData(const QString &username) const {
	UserData userData;
	UserData::findByName(username, &userData);

	return m_userData.value(userData.id());
}
