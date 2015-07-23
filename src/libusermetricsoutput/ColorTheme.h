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

#include <QtCore/QObject>
#include <QtGui/QColor>

/**
 * @{
 **/

namespace UserMetricsOutput {

/**
 * @brief ColorTheme for a particular user metric
 *
 * Each ColorTheme has a start, middle (main) and
 * end QColor.
 **/
class Q_DECL_EXPORT ColorTheme: public QObject {
Q_OBJECT

/**
 * @brief First / start QColor in the ColorTheme.
 */
Q_PROPERTY(QColor start READ start NOTIFY startChanged FINAL)

/**
 * @brief Middle / main QColor in the ColorTheme.
 */
Q_PROPERTY(QColor main READ main NOTIFY mainChanged FINAL)

/**
 * @brief Last / end QColor in the ColorTheme.
 */
Q_PROPERTY(QColor end READ end NOTIFY endChanged FINAL)

protected:
	/**
	 * @brief Unusable constructor - class is pure-virtual.
	 *
	 * @param parent
	 */
	explicit ColorTheme(QObject *parent = 0);

public:
	/**
	 * @brief Destructor
	 */
	virtual ~ColorTheme();

	/**
	 * @brief First / start QColor in the ColorTheme.
	 */
	virtual QColor start() const = 0;

	/**
	 * @brief Middle / main QColor in the ColorTheme.
	 */
	virtual QColor main() const = 0;

	/**
	 * @brief Last / end QColor in the ColorTheme.
	 */
	virtual QColor end() const = 0;

Q_SIGNALS:
	/**
	 * @brief The first / start QColor has changed.
	 *
	 * @param color
	 */
	void startChanged(const QColor &color);

	/**
	 * @brief The middle / main QColor has changed.
	 *
	 * @param color
	 */
	void mainChanged(const QColor &color);

	/**
	 * @brief The first / end QColor has changed.
	 *
	 * @param color
	 */
	void endChanged(const QColor &color);

};

}

/*@}*/

#endif // USERMETRICSOUTPUT_COLORTHEME_H_
