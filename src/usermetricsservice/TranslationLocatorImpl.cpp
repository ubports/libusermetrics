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

#define QT_NO_SIGNALS_SLOTS_KEYWORDS 1

#include <usermetricsservice/TranslationLocatorImpl.h>

#include <QDebug>
#include <QDir>
#include <QSharedPointer>

#include <click-0.4/click.h>

namespace UserMetricsService {

namespace {
QString pkgdir(const QString& id) {
	QSharedPointer<ClickDB> db(click_db_new(), &g_object_unref);

	GError *error = nullptr;
	click_db_read(db.data(), nullptr, &error);
	if (error != nullptr) {
		qWarning() << error->message;
		g_error_free(error);
		return QString();
	}

	error = nullptr;
	char *path = click_db_get_path(db.data(), id.toUtf8().constData(), "current", &error);
	if (error != nullptr) {
		qWarning() << error->message;
		g_error_free(error);
		return QString();
	}

	QString result = QString::fromUtf8(path);
	g_free(path);

	return result;
}
}

TranslationLocatorImpl::TranslationLocatorImpl() {
}

QString TranslationLocatorImpl::locate(const QString& id) {
	QString translationPath = "/usr/share/locale-langpack";
	if (id != "unconfined") {
		translationPath = QDir(pkgdir(id)).filePath("share/locale");
	}
	return translationPath;
}

}
