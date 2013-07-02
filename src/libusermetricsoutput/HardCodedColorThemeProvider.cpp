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

#include <libusermetricsoutput/HardCodedColorThemeProvider.h>
#include <libusermetricsoutput/ColorThemeImpl.h>

#include <QtCore/QString>

using namespace std;
using namespace UserMetricsOutput;

HardCodedColorThemeProvider::HardCodedColorThemeProvider(QObject *parent) :
		ColorThemeProvider(parent) {
	QColor orange(QColor::fromRgbF(0.9, 0.3, 0.1));
	QColor yellow(QColor::fromRgbF(1.0, 0.6, 0.0));
	QColor red(QColor::fromRgbF(0.8, 0.0, 0.0));
	QColor darkPurple(QColor::fromRgbF(0.5, 0.2, 0.3));
	QColor lightPurple(QColor::fromRgbF(0.8, 0.1, 0.8));
	QColor pink(QColor::fromRgbF(0.75, 0.13, 0.75));

	ColorThemePtr orangeTheme(
			ColorThemePtr(new ColorThemeImpl(yellow, orange, red)));
	ColorThemePtr yellowTheme(
			ColorThemePtr(new ColorThemeImpl(orange, yellow, orange)));
	ColorThemePtr redTheme(ColorThemePtr(new ColorThemeImpl(red, red, red)));
	ColorThemePtr darkPurpleTheme(
			ColorThemePtr(new ColorThemeImpl(lightPurple, darkPurple, pink)));
	ColorThemePtr lightPurpleTheme(
			ColorThemePtr(
					new ColorThemeImpl(lightPurple, lightPurple, lightPurple)));
	ColorThemePtr pinkTheme(
			ColorThemePtr(new ColorThemeImpl(lightPurple, pink, darkPurple)));

	m_colorThemes << ColorThemePtrPair(yellowTheme, orangeTheme);
	m_colorThemes << ColorThemePtrPair(pinkTheme, orangeTheme);
	m_colorThemes << ColorThemePtrPair(darkPurpleTheme, redTheme);
	m_colorThemes << ColorThemePtrPair(lightPurpleTheme, darkPurpleTheme);

	m_color = m_colorThemes.begin();
}

HardCodedColorThemeProvider::~HardCodedColorThemeProvider() {
}

ColorThemePtrPair HardCodedColorThemeProvider::getColorTheme(
		const QString &dataSetId) {
	ColorThemePtrPair result;

	map_const_iterator it(m_colorThemeMap.constFind(dataSetId));
	if (it == m_colorThemeMap.constEnd()) {
		// get the next available color
		result = *m_color;

		++m_color;
		if (m_color == m_colorThemes.end()) {
			m_color = m_colorThemes.begin();
		}

		m_colorThemeMap.insert(dataSetId, result);
	} else {
		// there was a mapped value
		result = *it;
	}

	return result;
}
