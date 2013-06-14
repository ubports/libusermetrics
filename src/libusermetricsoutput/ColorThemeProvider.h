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

#ifndef USERMETRICSOUTPUT_COLORTHEMEPROVIDER_H_
#define USERMETRICSOUTPUT_COLORTHEMEPROVIDER_H_

#include <libusermetricsoutput/ColorTheme.h>

#include <QtCore/QObject>
#include <QtCore/QString>

namespace UserMetricsOutput {

class ColorThemeProvider: public QObject {
protected:
	ColorThemeProvider(QObject *parent = 0);

public:
	typedef std::pair<const ColorTheme &, const ColorTheme &> ColorThemeRefPair;

	virtual ~ColorThemeProvider();

	virtual ColorThemeRefPair getColorTheme(const QString& dataSetId) = 0;
};

}
#endif // USERMETRICSOUTPUT_COLORTHEMEPROVIDER_H_
