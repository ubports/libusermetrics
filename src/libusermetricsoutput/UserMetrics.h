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

#ifndef USERMETRICSOUTPUT_USERMETRICS_H_
#define USERMETRICSOUTPUT_USERMETRICS_H_

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtCore/QAbstractItemModel>

#include <libusermetricsoutput/ColorTheme.h>

/*@{*/

namespace UserMetricsOutput {

/**
 * @brief Anonymous User Metrics
 **/
class Q_DECL_EXPORT UserMetrics: public QObject {
Q_OBJECT

	/**
	 * @brief Insert documentation here.
	 */
Q_PROPERTY(QString label READ label NOTIFY labelChanged FINAL)

/**
 * @brief Insert documentation here.
 */
Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)

/**
 * @brief Insert documentation here.
 */
Q_PROPERTY(UserMetricsOutput::ColorTheme* firstColor READ firstColor NOTIFY firstColorChanged FINAL)

/**
 * @brief Insert documentation here.
 */
Q_PROPERTY(UserMetricsOutput::ColorTheme* secondColor READ secondColor NOTIFY secondColorChanged FINAL)

/**
 * @brief Insert documentation here.
 */
Q_PROPERTY(QAbstractItemModel *firstMonth READ firstMonth NOTIFY firstMonthChanged FINAL)

/**
 * @brief Insert documentation here.
 */
Q_PROPERTY(QAbstractItemModel *secondMonth READ secondMonth NOTIFY secondMonthChanged FINAL)

/**
 * @brief Insert documentation here.
 */
Q_PROPERTY(int currentDay READ currentDay NOTIFY currentDayChanged FINAL)

public:
	/**
	 * @brief Insert documentation here.
	 */
	static UserMetrics *getInstance();

	/**
	 * @brief Insert documentation here.
	 */
	virtual ~UserMetrics();

	/**
	 * @brief Insert documentation here.
	 */
	virtual QString label() const = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual QString username() const = 0;

	/**
	 * @brief Insert documentation here.
	 *
	 * @param username Foo
	 */
	virtual void setUsername(const QString &username) = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual ColorTheme * firstColor() const = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual QAbstractItemModel *firstMonth() const = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual int currentDay() const = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual ColorTheme * secondColor() const = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual QAbstractItemModel *secondMonth() const = 0;

Q_SIGNALS:
	/**
	 * @brief Insert documentation here.
	 *
	 * @param label Foo
	 */
	void labelChanged(const QString &label);

	/**
	 * @brief Insert documentation here.
	 *
	 * @param username Foo
	 */
	void usernameChanged(const QString &username);

	/**
	 * @brief Insert documentation here.
	 *
	 * @param color Foo
	 */
	void firstColorChanged(ColorTheme *color);

	/**
	 * @brief Insert documentation here.
	 *
	 * @param firstMonth Foo
	 */
	void firstMonthChanged(QAbstractItemModel *firstMonth);

	/**
	 * @brief Insert documentation here.
	 *
	 * @param length Foo
	 */
	void currentDayChanged(int length);

	/**
	 * @brief Insert documentation here.
	 *
	 * @param color Foo
	 */
	void secondColorChanged(ColorTheme *color);

	/**
	 * @brief Insert documentation here.
	 *
	 * @param secondMonth Foo
	 */
	void secondMonthChanged(QAbstractItemModel *secondMonth);

	/**
	 * @brief Insert documentation here.
	 */
	void nextDataSource();

	/**
	 * @brief Insert documentation here.
	 */
	void readyForDataChange();

	/**
	 * @brief Insert documentation here.
	 */
	void dataAboutToAppear();

	/**
	 * @brief Insert documentation here.
	 */
	void dataAppeared();

	/**
	 * @brief Insert documentation here.
	 */
	void dataAboutToChange();

	/**
	 * @brief Insert documentation here.
	 */
	void dataChanged();

	/**
	 * @brief Insert documentation here.
	 */
	void dataAboutToDisappear();

	/**
	 * @brief Insert documentation here.
	 */
	void dataDisappeared();

public Q_SLOTS:
	/**
	 * @brief Insert documentation here.
	 */
	virtual void nextDataSourceSlot() = 0;

	/**
	 * @brief Insert documentation here.
	 */
	virtual void readyForDataChangeSlot() = 0;

protected:
	/**
	 * @brief Insert documentation here.
	 *
	 * @param parent Foo
	 */
	explicit UserMetrics(QObject *parent = 0);

	Q_DISABLE_COPY(UserMetrics)

};

}

/*@}*/

#endif // USERMETRICSOUTPUT_USERMETRICS_H_
