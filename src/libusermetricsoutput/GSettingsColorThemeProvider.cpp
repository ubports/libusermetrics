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

#include <libusermetricsoutput/GSettingsColorThemeProvider.h>
#include <libusermetricsoutput/ColorThemeImpl.h>

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QGSettings/QGSettings>

using namespace std;
using namespace UserMetricsOutput;

GSettingsColorThemeProvider::GSettingsColorThemeProvider(QObject *parent) :
		ColorThemeProvider(parent) {

	if (qEnvironmentVariableIsSet("USERMETRICS_NO_LOAD_COLORS")) {
		ColorThemePtr blankTheme(
				ColorThemePtr(
						new ColorThemeImpl(QColor(), QColor(), QColor())));
		m_colorThemes << ColorThemePtrPair(blankTheme, blankTheme);
	} else {
		QGSettings settings("com.canonical.UserMetrics",
				"/com/canonical/UserMetrics/");

		QString foregroundKey("theme-%1-foreground");
		QString backgroundKey("theme-%1-background");

		for (uint id(1); id <= 20; ++id) {
			QStringList foreground(
					settings.get(foregroundKey.arg(id)).toStringList());
			QStringList background(
					settings.get(backgroundKey.arg(id)).toStringList());

			// skip incorrectly sized arrays
			if (foreground.size() != 3 || background.size() != 3) {
				continue;
			}

			ColorThemePtr foregroundTheme(
					ColorThemePtr(
							new ColorThemeImpl(QColor(foreground[0]),
									QColor(foreground[1]),
									QColor(foreground[2]))));
			ColorThemePtr backgroundTheme(
					ColorThemePtr(
							new ColorThemeImpl(QColor(background[0]),
									QColor(background[1]),
									QColor(background[2]))));

			m_colorThemes
					<< ColorThemePtrPair(foregroundTheme, backgroundTheme);
		}
	}

	m_color = m_colorThemes.begin();
}

GSettingsColorThemeProvider::~GSettingsColorThemeProvider() {
}

ColorThemePtrPair GSettingsColorThemeProvider::getColorTheme(
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
