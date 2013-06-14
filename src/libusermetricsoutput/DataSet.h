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

#ifndef USERMETRICSOUTPUT_DATASET_H_
#define USERMETRICSOUTPUT_DATASET_H_

#include <libusermetricsoutput/ColorThemeImpl.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QVariantList>
#include <QtCore/QDate>

namespace UserMetricsOutput {

class DataSet: public QObject {
Q_OBJECT

Q_PROPERTY(QDate lastUpdated READ lastUpdated NOTIFY lastUpdatedChanged FINAL)
Q_PROPERTY(const QVariantList data READ data NOTIFY dataChanged FINAL)
Q_PROPERTY(QString formatString READ formatString WRITE setFormatString NOTIFY formatStringChanged FINAL)

public:
	DataSet(QObject* parent);

	~DataSet();

	const QString & formatString() const;

	const QVariantList & data() const;

	const QDate & lastUpdated() const;

	void setData(const QDate &lastUpdated, const QVariantList &data);

public Q_SLOTS:
	void setFormatString(const QString &formatString);

Q_SIGNALS:
	void lastUpdatedChanged(const QDate &lastUpdated);

	void dataChanged(const QVariantList *data);

	void formatStringChanged(const QString &formatString);

protected:
	QString m_formatString;

	QDate m_lastUpdated;

	QVariantList m_data;
};

}

#endif // USERMETRICSOUTPUT_DATASET_H_
