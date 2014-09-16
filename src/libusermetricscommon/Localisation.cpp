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

#include <libusermetricscommon/Localisation.h>
#include <QtCore/QProcess>

QString gettextExternal(const QString &textDomain, const QString &messageId,
		const QString &localeDir) {
	QProcess gettext;

	if (!localeDir.isEmpty()) {
		QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
		env.insert("TEXTDOMAINDIR", localeDir);
		gettext.setProcessEnvironment(env);
	}

	gettext.setProcessChannelMode(QProcess::ForwardedErrorChannel);
	gettext.start("gettext", QStringList() << textDomain << messageId);
	gettext.waitForReadyRead(200);
	QByteArray ba(gettext.readAllStandardOutput());
	gettext.kill();

	return QString::fromUtf8(ba);
}
