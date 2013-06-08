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

#include <QtCore/QSharedPointer>

#include <libusermetricsoutput/ColorThemeImpl.h>
#include <QtCore/QVariantList>

namespace UserMetricsOutput {

class DataSet: public QObject {
public:
	explicit DataSet(QObject *parent);

	DataSet(const QString &label, const ColorTheme &firstColor,
			const QVariantList &firstMonth, const ColorTheme &secondColor,
			const QVariantList &secondMonth, QObject* parent);

	~DataSet();

	const QString & label() const;

	const ColorTheme & firstColor() const;

	const QVariantList & firstMonth() const;

	const ColorTheme & secondColor() const;

	const QVariantList & secondMonth() const;

	int length() const;

protected:
	int calculateLength();

	QString m_label;
	ColorThemeImpl m_firstColor;
	QVariantList m_firstMonth;
	ColorThemeImpl m_secondColor;
	QVariantList m_secondMonth;
	int m_length;
};

}

#endif // USERMETRICSOUTPUT_DATASET_H_
