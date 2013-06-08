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

#ifndef USERMETRICSOUTPUT_COLORTHEMEIMPL_H_
#define USERMETRICSOUTPUT_COLORTHEMEIMPL_H_

#include <libusermetricsoutput/ColorTheme.h>

namespace UserMetricsOutput {

class ColorThemeImpl: public ColorTheme {
public:
	explicit ColorThemeImpl(QObject *parent = 0);

	ColorThemeImpl(const QColor &start, const QColor &main, const QColor &end,
			QObject *parent = 0);

	virtual ~ColorThemeImpl();

	virtual QColor start() const;

	virtual QColor main() const;

	virtual QColor end() const;

	virtual void setStart(const QColor &color);

	virtual void setMain(const QColor &color);

	virtual void setEnd(const QColor &color);

	virtual void setColors(const ColorTheme &other);

protected:
	QColor m_start;

	QColor m_main;

	QColor m_end;

	int calculateLength();
};

}

#endif /* USERMETRICSOUTPUT_COLORTHEMEIMPL_H_ */
