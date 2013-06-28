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

#include <usermetricsservice/database/UserData.h>

#include <QDjangoQuerySet.h>

using namespace UserMetricsService;

UserData::UserData(QObject *parent) :
		QDjangoModel(parent), m_id(0) {
}

UserData::~UserData() {
}

int UserData::id() const {
	return m_id;
}

void UserData::setId(int id) {
	m_id = id;
}

const QString & UserData::username() const {
	return m_username;
}

void UserData::setUsername(const QString &username) {
	m_username = username;
}

void UserData::findById(int id, UserData *userData) {
	QDjangoQuerySet<UserData>().get(
			QDjangoWhere("id", QDjangoWhere::Equals, id), userData);
}

void UserData::findByName(const QString &username, UserData *userData) {
	QDjangoQuerySet<UserData>().get(
			QDjangoWhere("username", QDjangoWhere::Equals, username), userData);
}
