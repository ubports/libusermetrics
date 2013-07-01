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

#include <QtCore/QCoreApplication>
#include <QDjangoQuerySet.h>
#include <QSqlDatabase>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);

	QString databaseName("/tmp/usermetrics.db");
	QStringList arguments(application.arguments());
	if (arguments.size() == 2) {
		databaseName = arguments.at(1);
	}

	// Database setup
	QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
	db.setDatabaseName(databaseName);
	if (!db.open()) {
		throw exception();
	}

	QDjango::setDatabase(db);

	QDBusConnection connection(QDBusConnection::systemBus());

	QSharedPointer<DateFactory> dateFactory(new DateFactoryImpl());
	DBusUserMetrics userMetrics(connection, dateFactory);

	bool result(application.exec());

	db.close();

	return result;
}
