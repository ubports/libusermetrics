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

/*@{*/

namespace UserMetricsOutput {

/**
 * @brief Color theme for a particular user metric
 **/
class Q_DECL_EXPORT ColorTheme: public QObject {
Q_OBJECT

	/**
	 * @brief Insert documentation here.
	 */
Q_PROPERTY(QColor start READ start NOTIFY startChanged FINAL)

/**
 * @brief Insert documentation here.
 */
Q_PROPERTY(QColor main READ main NOTIFY mainChanged FINAL)

/**
 * @brief Insert documentation here.
 */
Q_PROPERTY(QColor end READ end NOTIFY endChanged FINAL)

protected:
	/**
	 * @brief Insert documentation here.
	 *
	 * @param parent Foo
	 */
	explicit ColorTheme(QObject *parent = 0);

public:
	virtual ~ColorTheme();

	/**
	 * @brief Insert documentation here.
	 */
	virtual QColor start() const = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual QColor main() const = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual QColor end() const = 0;

Q_SIGNALS:
	/**
	 * @brief Insert documentation here.
	 *
	 * @param color Foo
	 */
	void startChanged(const QColor &color);

	/**
	 * @brief Insert documentation here.
	 *
	 * @param color Foo
	 */
	void mainChanged(const QColor &color);

	/**
	 * @brief Insert documentation here.
	 *
	 * @param color Foo
	 */
	void endChanged(const QColor &color);

};

}

/*@}*/

#endif // USERMETRICSOUTPUT_COLORTHEME_H_
