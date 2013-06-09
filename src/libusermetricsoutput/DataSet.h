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

public:
	DataSet(const QString &formatString, const ColorTheme &firstColor,
			const ColorTheme &secondColor, QObject* parent);

	~DataSet();

	const QString & formatString() const;

	const ColorTheme & firstColor() const;

	const ColorTheme & secondColor() const;

	const QVariantList & data() const;

	const QDate & lastUpdated() const;

	void setData(const QDate &lastUpdated, const QVariantList &data);

Q_SIGNALS:
	void lastUpdatedChanged(const QDate &lastUpdated);

	void dataChanged(const QVariantList *data);

protected:
	QString m_formatString;

	ColorThemeImpl m_firstColor;

	ColorThemeImpl m_secondColor;

	QDate m_lastUpdated;

	QVariantList m_data;
};

}

#endif // USERMETRICSOUTPUT_DATASET_H_