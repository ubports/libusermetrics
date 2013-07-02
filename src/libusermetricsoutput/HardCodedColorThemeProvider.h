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

#ifndef USERMETRICSOUTPUT_HARDCODEDCOLORTHEMEPROVIDER_H_
#define USERMETRICSOUTPUT_HARDCODEDCOLORTHEMEPROVIDER_H_

#include <libusermetricsoutput/ColorTheme.h>
#include <libusermetricsoutput/ColorThemeProvider.h>

#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QSharedPointer>

namespace UserMetricsOutput {

class HardCodedColorThemeProvider: public ColorThemeProvider {
public:

	typedef QVector<ColorThemePtrPair> ColorThemeList;

	typedef ColorThemeList::const_iterator const_interator;

	typedef QMap<QString, ColorThemePtrPair> ColorThemeMap;

	typedef ColorThemeMap::const_iterator map_const_iterator;

	HardCodedColorThemeProvider(QObject *parent = 0);

	virtual ~HardCodedColorThemeProvider();

	virtual ColorThemePtrPair getColorTheme(const QString& dataSetId);

protected:
	ColorThemeList m_colorThemes;

	const_interator m_color;

	ColorThemeMap m_colorThemeMap;
};

}
#endif // USERMETRICSOUTPUT_HARDCODEDCOLORTHEMEPROVIDER_H_
