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

#include <libusermetricsinput/MetricManager.h>
#include <libusermetricscommon/Localisation.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

using namespace std;
using namespace UserMetricsInput;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);

	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	textdomain(GETTEXT_PACKAGE);

	if (argc < 6) {
		qWarning() << _("Usage: ") << argv[0] << " "
				<< _(
						"DATA_SOURCE_ID FORMAT_STRING EMPTY_DATA_STRING USERNAME <DATA>");
		return 1;
	}

	QString dataSourceId(QString::fromUtf8(argv[1]));
	QString formatString(QString::fromUtf8(argv[2]));
	QString emptyDataString(QString::fromUtf8(argv[3]));
	QString username(QString::fromUtf8(argv[4]));

	MetricManagerPtr manager(MetricManager::getInstance());
	MetricPtr metric(manager->add(dataSourceId, formatString, emptyDataString));
	MetricUpdatePtr update(metric->update(username));

	for (int i(5); i < argc; ++i) {
		double data(stod(argv[i]));
		update->addData(data);
	}

	return 0;
}
