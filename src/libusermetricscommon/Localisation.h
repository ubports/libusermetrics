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

#ifndef USERMETRICSCOMMON_LOCALISATION_H_
#define USERMETRICSCOMMON_LOCALISATION_H_

#include <libintl.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QThread>

inline char * _(const char *__msgid) {
	return dgettext(GETTEXT_PACKAGE, __msgid);
}

inline char * _(const char *__msgid, int __category) {
	return dcgettext(GETTEXT_PACKAGE, __msgid, __category);
}

inline char * _(const char *__msgid1,
		const char *__msgid2, unsigned long int __n) {
	return dngettext(GETTEXT_PACKAGE, __msgid1, __msgid2, __n);
}

inline char * _(const char *__msgid1,
		const char *__msgid2, unsigned long int __n, int __category) {
	return dcngettext(GETTEXT_PACKAGE, __msgid1, __msgid2, __n, __category);
}

class GettextRequest: public QObject {
Q_OBJECT

public:
	typedef QSharedPointer<GettextRequest> Ptr;

	QString m_textDomain;
	QString m_messageId;
	QString m_localeDir;
	QString m_requestId;

Q_SIGNALS:
	void result(const QString &result, const QString &requestId);
};


class ExternalGettext: public QObject {
Q_OBJECT

public:
	typedef QSharedPointer<ExternalGettext> Ptr;

	static Ptr singletonInstance();

	ExternalGettext();

	~ExternalGettext();

public Q_SLOTS:
	void tr(const QString &textDomain, const QString &messageId,
			const QString &localeDir, const QObject *who,
			const QString &requestId);

Q_SIGNALS:
	void newRequest(GettextRequest::Ptr request);

protected:
	class Priv;

	QThread m_thread;
	QSharedPointer<Priv> p;
};

Q_DECLARE_METATYPE(GettextRequest::Ptr)

#endif // USERMETRICSCOMMON_LOCALISATION_H_
