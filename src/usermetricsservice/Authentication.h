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

#ifndef USERMETRICSSERVICE_AUTHENTICATION_H_
#define USERMETRICSSERVICE_AUTHENTICATION_H_

#include <QtCore/QString>
#include <QtDBus/QDBusError>

QT_BEGIN_NAMESPACE
class QDBusConnection;
class QDBusContext;
QT_END_NAMESPACE

namespace UserMetricsService {

class Authentication {
public:
	Authentication();

	virtual ~Authentication();

	virtual QString getConfinementContext(const QDBusContext &context) const;

	virtual QString getUsername(const QDBusContext &context) const;

	virtual void sendErrorReply(const QDBusContext &context,
			QDBusError::ErrorType type, const QString &msg = QString()) const;
};

}
#endif // USERMETRICSSERVICE_AUTHENTICATION_H_
