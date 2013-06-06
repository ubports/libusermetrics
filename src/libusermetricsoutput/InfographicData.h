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

#ifndef USERMETRICSOUTPUT_INFOGRAPHICDATA_H
#define USERMETRICSOUTPUT_INFOGRAPHICDATA_H

#include <QtCore/QSharedPointer>

#include <libusermetricsoutput/InfographicColorThemeImpl.h>
#include <libusermetricsoutput/qvariantlistmodel.h>

namespace UserMetricsOutput {

class InfographicData: public QObject {
public:
	explicit InfographicData(QObject *parent);

	InfographicData(const QString &label,
			const InfographicColorTheme &firstColor,
			const QVariantList &firstMonth,
			const InfographicColorTheme &secondColor,
			const QVariantList &secondMonth, QObject* parent);

	~InfographicData();

	const QString & label() const;
	const InfographicColorTheme & firstColor() const;
	const QVariantList & firstMonth() const;
	const InfographicColorTheme & secondColor() const;
	const QVariantList & secondMonth() const;
	int length() const;

protected:
	int calculateLength();

	QString m_label;
	InfographicColorThemeImpl m_firstColor;
	QVariantList m_firstMonth;
	InfographicColorThemeImpl m_secondColor;
	QVariantList m_secondMonth;
	int m_length;

//	Q_DECLARE_PRIVATE(InfographicData)
};

}

#endif // USERMETRICSOUTPUT_INFOGRAPHICDATA_H
