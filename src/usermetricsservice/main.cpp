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

#include <usermetricsservice/Authentication.h>
#include <usermetricsservice/DBusUserMetrics.h>
#include <usermetricsservice/TranslationLocatorImpl.h>
#include <libusermetricscommon/DateFactoryImpl.h>
#include <libusermetricscommon/DBusPaths.h>
#include <libusermetricscommon/Localisation.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include <QDjangoQuerySet.h>
#include <QSqlDatabase>

#include <csignal>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

static void exitQt(int sig) {
	Q_UNUSED(sig);
	QCoreApplication::exit(0);
}

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);

	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	textdomain(GETTEXT_PACKAGE);

	QString databaseName("/var/lib/usermetrics/usermetrics6.db");
	QStringList arguments(application.arguments());
	if (arguments.size() == 2) {
		databaseName = arguments.at(1);
	}

	// Database setup
	QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
	db.setDatabaseName(databaseName);
	if (!db.open()) {
		qWarning() << _("Could not open database") << " [" << databaseName
				<< "]";
		return 1;
	}

	if(qEnvironmentVariableIsSet("USERMETRICS_DATABASE_DEBUG")) {
		QDjango::setDebugEnabled(true);
	}

	QDjango::setDatabase(db);

	QDBusConnection connection(QDBusConnection::systemBus());

	QSharedPointer<DateFactory> dateFactory(new DateFactoryImpl());
	QSharedPointer<Authentication> authentication(new Authentication());
	QSharedPointer<TranslationLocator> translationLocator(new TranslationLocatorImpl());

	DBusUserMetrics userMetrics(connection, dateFactory, authentication, translationLocator);
	if (!connection.registerService(DBusPaths::serviceName())) {
		qWarning() << _("Unable to register user metrics service on DBus");
		return 1;
	}

	signal(SIGINT, &exitQt);
	signal(SIGTERM, &exitQt);

	int result(0);
	try {
		result = application.exec();
	} catch (std::logic_error &e) {
		qWarning() << "User metrics service error:" << e.what();
	}
	if (!connection.unregisterService(DBusPaths::serviceName())) {
		qWarning() << _("Unable to unregister user metrics service on DBus");
	}

	db.close();

	return result;
}
