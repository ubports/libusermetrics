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
	QVector<QColor> colors;
	colors.push_back(QColor::fromRgbF(0.3, 0.27, 0.32));
	colors.push_back(QColor::fromRgbF(0.83, 0.49, 0.58));
	colors.push_back(QColor::fromRgbF(0.63, 0.51, 0.59));

	colors.push_back(QColor::fromRgbF(0.28, 0.26, 0.4));
	colors.push_back(QColor::fromRgbF(0.47, 0.38, 0.56));
	colors.push_back(QColor::fromRgbF(0.69, 0.65, 0.78));

	colors.push_back(QColor::fromRgbF(0.32, 0.21, 0.16));
	colors.push_back(QColor::fromRgbF(0.55, 0.45, 0.32));
	colors.push_back(QColor::fromRgbF(0.85, 0.74, 0.53));

	colors.push_back(QColor::fromRgbF(0.25, 0.31, 0.19));
	colors.push_back(QColor::fromRgbF(0.63, 0.53, 0.3));
	colors.push_back(QColor::fromRgbF(0.89, 0.56, 0.31));

	colors.push_back(QColor::fromRgbF(0.17, 0.34, 0.36));
	colors.push_back(QColor::fromRgbF(0.36, 0.54, 0.49));
	colors.push_back(QColor::fromRgbF(0.65, 0.76, 0.64));

	colors.push_back(QColor::fromRgbF(0.21, 0.38, 0.37));
	colors.push_back(QColor::fromRgbF(0.35, 0.49, 0.5));
	colors.push_back(QColor::fromRgbF(0.67, 0.75, 0.82));

	ColorThemePtr first(
			ColorThemePtr(new ColorThemeImpl(colors[0], colors[1], colors[2])));
	ColorThemePtr second(
			ColorThemePtr(new ColorThemeImpl(colors[3], colors[4], colors[5])));
	ColorThemePtr eighth(
			ColorThemePtr(new ColorThemeImpl(colors[6], colors[7], colors[8])));
	ColorThemePtr ninth(
			ColorThemePtr(
					new ColorThemeImpl(colors[9], colors[10], colors[11])));
	ColorThemePtr eleventh(
			ColorThemePtr(
					new ColorThemeImpl(colors[12], colors[13], colors[14])));
	ColorThemePtr twelfth(
			ColorThemePtr(
					new ColorThemeImpl(colors[15], colors[16], colors[17])));

	m_colorThemes << ColorThemePtrPair(first, second);

	m_color = m_colorThemes.begin();
}

HardCodedColorThemeProvider::~HardCodedColorThemeProvider() {
}

ColorThemeProvider::ColorThemeRefPair HardCodedColorThemeProvider::getColorTheme(
		const QString &dataSetId) {
	Q_UNUSED(dataSetId);

	ColorThemePtrPair current(*m_color);
	ColorThemeRefPair result(*current.first, *current.second);

	++m_color;
	if (m_color == m_colorThemes.end()) {
		m_color = m_colorThemes.begin();
	}

	return result;
}
