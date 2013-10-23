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
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QXmlStreamReader>
#include <QGSettings/QGSettings>
#include <QtXmlPatterns/QXmlSchema>

namespace UserMetricsOutput {

class GSettingsColorThemeProvider: public ColorThemeProvider {
Q_OBJECT
public:
	typedef QVector<ColorThemePtrPair> ColorThemeList;

	typedef ColorThemeList::const_iterator const_interator;

	typedef QMap<QString, ColorThemePtrPair> ColorThemeMap;

	typedef ColorThemeMap::const_iterator map_const_iterator;

	GSettingsColorThemeProvider(QObject *parent = 0);

	virtual ~GSettingsColorThemeProvider();

	virtual ColorThemePtrPair getColorTheme(const QString& dataSetId);

protected Q_SLOTS:
	void changed(const QString &key);

protected:
	void loadXmlColors(const QString &theme);

	void loadBlankColors();

	void parseTheme(QXmlStreamReader & xml);

	QString convertPath(const QString &base, const QString& theme);

	QStringList m_baseDirs;

	QXmlSchema m_schema;

	ColorThemeList m_colorThemes;

	const_interator m_color;

	ColorThemeMap m_colorThemeMap;

	QScopedPointer<QGSettings> m_settings;
};

}
#endif // USERMETRICSOUTPUT_HARDCODEDCOLORTHEMEPROVIDER_H_
