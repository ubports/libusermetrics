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

#include <libusermetricsoutput/DateFactoryImpl.h>
#include <libusermetricsoutput/UserMetricsImpl.h>

using namespace UserMetricsOutput;

UserMetrics::UserMetrics(QObject *parent) :
		QObject(parent) {
}

UserMetrics::~UserMetrics() {
}

/**
 * Factory methods
 */

UserMetrics * UserMetrics::getInstance() {
	return new UserMetricsImpl(
			QSharedPointer<DateFactory>(new DateFactoryImpl()));
}
