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

#ifndef USERMETRICSOUTPUT_DATASOURCE_H_
#define USERMETRICSOUTPUT_DATASOURCE_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>

namespace UserMetricsOutput {

class DataSource;

typedef QSharedPointer<DataSource> DataSourcePtr;

enum MetricType {
	USER, SYSTEM
};

class DataSource: public QObject {
Q_OBJECT

Q_PROPERTY(QString formatString READ formatString WRITE setFormatString NOTIFY formatStringChanged FINAL)

Q_PROPERTY(QString emptyDataString READ emptyDataString WRITE setEmptyDataString NOTIFY emptyDataStringChanged FINAL)

Q_PROPERTY(QString textDomain READ textDomain WRITE setTextDomain NOTIFY textDomainChanged FINAL)

public:
	explicit DataSource(const QString &localeDir = LOCALEDIR, QObject *parent =
			0);

	virtual ~DataSource();

	const QString & formatString() const;

	const QString & emptyDataString() const;

	const QString & textDomain() const;

public Q_SLOTS:
	void setFormatString(const QString &formatString);

	void setEmptyDataString(const QString &emptyDataString);

	void setTextDomain(const QString &textDomain);

Q_SIGNALS:
	void formatStringChanged(const QString &formatString);

	void emptyDataStringChanged(const QString &emptyDataString);

	void textDomainChanged(const QString &textDomain);

protected:
	void updateFormatStringTranslation();

	void updateEmptyDataStringTranslation();

	QString m_formatString;

	QString m_formatStringTr;

	QString m_emptyDataString;

	QString m_emptyDataStringTr;

	QString m_textDomain;

	QString m_localeDir;
};

}

#endif // USERMETRICSOUTPUT_DATASOURCE_H_
