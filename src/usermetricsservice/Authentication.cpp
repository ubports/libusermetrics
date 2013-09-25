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

#include <usermetricsservice/Authentication.h>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusContext>
#include <pwd.h>
#include <sys/apparmor.h>

namespace UserMetricsService {

Authentication::Authentication() {
}

Authentication::~Authentication() {
}

QString Authentication::getConfinementContext(
		const QDBusContext& context) const {
	if (!context.calledFromDBus()
			|| qEnvironmentVariableIsSet("USERMETRICS_NO_AUTH")) {
		return "unconfined";
	}

	const QDBusConnectionInterface &interface(
			*context.connection().interface());
	unsigned int servicePid = interface.servicePid(context.message().service());

	char *con(0);
	aa_gettaskcon(servicePid, &con, 0);
	QString confinementContext(QString::fromUtf8(con));
	free(con);

	return confinementContext;
}

QString Authentication::getUsername(const QDBusContext& context) const {
	if (!context.calledFromDBus()
			|| qEnvironmentVariableIsSet("USERMETRICS_NO_AUTH")) {
		return "";
	}

	const QDBusConnectionInterface &interface(
			*context.connection().interface());
	unsigned int serviceUid = interface.serviceUid(context.message().service());

	struct passwd* pwd;
	char x_buf[1024 * sizeof(*pwd)];
	size_t size = sizeof(x_buf);
	char* buf = x_buf;

	int x_errno = getpwuid_r(serviceUid, (struct passwd*) (buf),
			buf + sizeof(*pwd), size - sizeof(*pwd), &pwd);
	if (x_errno) {
		pwd = 0;
		return "";
	}

	QString username(pwd->pw_name);

	if (buf != x_buf) {
		delete[] buf;
	}

	return username;
}

void Authentication::sendErrorReply(const QDBusContext &context,
		QDBusError::ErrorType type, const QString &msg) const {
	if (context.calledFromDBus()) {
		context.sendErrorReply(type, msg);
	}
}

} /* namespace UserMetricsService */
