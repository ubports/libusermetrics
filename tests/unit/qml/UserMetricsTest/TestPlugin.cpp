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
 */

#include <QtQuick/QtQuick>
#include <TestPlugin.h>
#include <MetricInfo.h>

void Components::registerTypes(const char *uri) {
	qmlRegisterUncreatableType<DBusQuery>(uri, 0, 1, "DBusQuery",
			"Use context singleton dbusQuery instead.");
	qmlRegisterUncreatableType<MetricInfo>(uri, 0, 1, "MetricInfo",
			"Obtain instances from DBusQuery methods.");
}

void Components::initializeEngine(QQmlEngine *engine, const char *uri) {
	m_dbusQuery.reset(new DBusQuery());
	QQmlExtensionPlugin::initializeEngine(engine, uri);
	engine->rootContext()->setContextProperty("dbusQuery", m_dbusQuery.data());
}
