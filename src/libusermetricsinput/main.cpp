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
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace UserMetricsInput;

int main(int argc, char *argv[]) {
	if (argc < 5) {
		cerr << "Usage: " << argv[0] << " DATA_SOURCE_ID FORMAT_STRING USERNAME <DATA>"
				<< endl;
		return 1;
	}

	string dataSourceId(argv[1]);
	string formatString(argv[2]);
	string username(argv[3]);

	MetricManagerPtr manager(MetricManager::getInstance());
	MetricPtr metric(manager->add(dataSourceId, formatString));
	MetricUpdatePtr update = metric->update(username);

	for (int i(4); i < argc; ++i) {
		double data(stod(string(argv[i])));
		update->addData(data);
	}

	return 0;
}
