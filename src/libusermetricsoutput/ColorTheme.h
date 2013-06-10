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

#ifndef USERMETRICSOUTPUT_COLORTHEME_H_
#define USERMETRICSOUTPUT_COLORTHEME_H_

#include <QtGui/QColor>

namespace UserMetricsOutput {

class Q_DECL_EXPORT ColorTheme: public QObject {
Q_OBJECT

Q_PROPERTY(QColor start READ start NOTIFY startChanged FINAL)
Q_PROPERTY(QColor main READ main NOTIFY mainChanged FINAL)
Q_PROPERTY(QColor end READ end NOTIFY endChanged FINAL)

public:
	explicit ColorTheme(QObject *parent = 0);

	virtual ~ColorTheme();

	virtual QColor start() const = 0;

	virtual QColor main() const = 0;

	virtual QColor end() const = 0;

Q_SIGNALS:
	void startChanged(const QColor &color);

	void mainChanged(const QColor &color);

	void endChanged(const QColor &color);
};

}

#endif // USERMETRICSOUTPUT_COLORTHEME_H_
