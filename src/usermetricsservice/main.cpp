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

#include <usermetricsservice/DBusUserMetrics.h>
#include <libusermetricscommon/DateFactoryImpl.h>
#include <libusermetricscommon/DBusPaths.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QDjangoQuerySet.h>
#include <QSqlDatabase>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
			QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	application.installTranslator(&qtTranslator);

	QTranslator myappTranslator;
	myappTranslator.load("usermetrics_" + QLocale::system().name());
	application.installTranslator(&myappTranslator);

	QString databaseName("/var/lib/usermetrics/usermetrics3.db");
	QStringList arguments(application.arguments());
	if (arguments.size() == 2) {
		databaseName = arguments.at(1);
	}

	// Database setup
	QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
	db.setDatabaseName(databaseName);
	if (!db.open()) {
		qWarning() << QCoreApplication::tr("Could not open database") << " ["
				<< databaseName << "]";
		return 1;
	}

	QDjango::setDatabase(db);

	QDBusConnection connection(QDBusConnection::systemBus());

	QSharedPointer<DateFactory> dateFactory(new DateFactoryImpl());

	if (!connection.registerService(DBusPaths::serviceName())) {
		qWarning()
				<< QCoreApplication::tr(
						"Unable to register user metrics service on DBus");
		return 1;
	}
	DBusUserMetrics userMetrics(connection, dateFactory);

	bool result(application.exec());
	if (!connection.unregisterService(DBusPaths::serviceName())) {
		qWarning()
				<< QCoreApplication::tr(
						"Unable to unregister user metrics service on DBus");
	}

	db.close();

	return result;
}
