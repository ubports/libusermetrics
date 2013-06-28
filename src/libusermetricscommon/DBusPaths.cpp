/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <libusermetricscommon/DBusPaths.h>

using namespace UserMetricsCommon;

DBusPaths::DBusPaths() {
}

DBusPaths::~DBusPaths() {
}

QString DBusPaths::serviceName() {
	return "com.canonical.UserMetrics";
}

QString DBusPaths::userMetrics() {
	return "/com/canonical/UserMetrics";
}

QString DBusPaths::userData(int id) {
	return QString("/com/canonical/UserMetrics/UserData/%1").arg(id);
}

QString DBusPaths::dataSource(int id) {
	return QString("/com/canonical/UserMetrics/DataSource/%1").arg(id);
}

QString DBusPaths::dataSet(int id) {
	return QString("/com/canonical/UserMetrics/DataSet/%1").arg(id);
}
