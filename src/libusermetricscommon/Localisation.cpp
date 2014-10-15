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
#include <QtCore/QDebug>
#include <QtCore/QProcess>

namespace {
static QWeakPointer<ExternalGettext> instance;
}

class ExternalGettext::Priv: public QObject {
Q_OBJECT

public Q_SLOTS:
	void tr(GettextRequest::Ptr request) {
		QProcess gettext;

		if (!request->m_localeDir.isEmpty()) {
			QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
			env.insert("TEXTDOMAINDIR", request->m_localeDir);
			gettext.setProcessEnvironment(env);
		}

		gettext.setProcessChannelMode(QProcess::ForwardedErrorChannel);
		gettext.start("gettext",
				QStringList() << request->m_textDomain << request->m_messageId);
		gettext.waitForReadyRead(200);
		QByteArray ba(gettext.readAllStandardOutput());
		gettext.waitForFinished(200);

		Q_EMIT request->result(QString::fromUtf8(ba), request->m_requestId);
	}
};

ExternalGettext::Ptr ExternalGettext::singletonInstance() {
	ExternalGettext::Ptr result(instance);

	if (!result) {
		qRegisterMetaType<GettextRequest::Ptr>("GettextRequest::Ptr");
		result.reset(new ExternalGettext);
		instance = result;
	}
	return result;
}

ExternalGettext::ExternalGettext() {
	m_thread.start();

	p.reset(new Priv);
	p->moveToThread(&m_thread);

	connect(this, SIGNAL(newRequest(GettextRequest::Ptr)),
			p.data(), SLOT(tr(GettextRequest::Ptr)),
			Qt::QueuedConnection);
}

ExternalGettext::~ExternalGettext() {
	if (m_thread.isRunning()) {
		m_thread.quit();
		m_thread.wait();
	}
}

void ExternalGettext::tr(const QString &textDomain, const QString &messageId,
		const QString &localeDir, const QObject *who,
		const QString &requestId) {

	GettextRequest::Ptr r(new GettextRequest);

	r->m_textDomain = textDomain;
	r->m_messageId = messageId;
	r->m_localeDir = localeDir;
	r->m_requestId = requestId;

	r->moveToThread(&m_thread);

	connect(r.data(),
			SIGNAL(result(const QString &, const QString &)),
			who,
			SLOT(translationResult(const QString &, const QString &)),
			Qt::QueuedConnection);

	Q_EMIT newRequest(r);
}

#include "Localisation.moc"
