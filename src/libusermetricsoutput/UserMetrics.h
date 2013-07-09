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

/**
 * @{
 **/

/**
 * @brief The user metrics output library namespace
 **/
namespace UserMetricsOutput {

/**
 * @brief Presentation API for user metrics.
 *
 * This class breaks down the various user metric sources
 * registered against different users into a presentable
 * format.
 *
 * The data is split into two "months" - "first" and "second".
 * The months always have a length equal to the size of
 * the month they represent - unset values are padded with
 * null data.
 *
 * The property currentDay indicates the current day's value
 * in the "first month" data.
 *
 * Given a username, the class then provides an API to
 * cycle through that user's data. The signal #nextDataSource
 * should be used for this.
 **/
class Q_DECL_EXPORT UserMetrics: public QObject {
Q_OBJECT

/**
 * @brief Represents a textual version of the current metric.
 *
 * e.g. "3 messages received today"
 */
Q_PROPERTY(QString label READ label NOTIFY labelChanged FINAL)

/**
 * @brief The current username selected.
 */
Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)

/**
 * @brief The ColorTheme for the first month.
 */
Q_PROPERTY(UserMetricsOutput::ColorTheme* firstColor READ firstColor NOTIFY firstColorChanged FINAL)

/**
 * @brief The ColorTheme for the second month.
 */
Q_PROPERTY(UserMetricsOutput::ColorTheme* secondColor READ secondColor NOTIFY secondColorChanged FINAL)

/**
 * @brief The data for the first month.
 */
Q_PROPERTY(QAbstractItemModel *firstMonth READ firstMonth NOTIFY firstMonthChanged FINAL)

/**
 * @brief The data for the second month.
 */
Q_PROPERTY(QAbstractItemModel *secondMonth READ secondMonth NOTIFY secondMonthChanged FINAL)

/**
 * @brief The current day of the calendar month.
 *
 * Zero-indexed.
 */
Q_PROPERTY(int currentDay READ currentDay NOTIFY currentDayChanged FINAL)

public:
	/**
	 * @brief Get a new instance of UserMetrics.
	 */
	static UserMetrics *getInstance();

	/**
	 * @brief Destructor.
	 */
	virtual ~UserMetrics();

	/**
	 * @brief Represents a textual version of the current metric.
	 *
	 * e.g. "3 messages received today"
	 */
	virtual QString label() const = 0;

	/**
	 * @brief The current username selected.
	 */
	virtual QString username() const = 0;

	/**
	 * @brief Change the current username.
	 *
	 * @param username
	 *
	 * The data source will change to the first one available
	 * for the given username.
	 */
	virtual void setUsername(const QString &username) = 0;

	/**
	 * @brief The ColorTheme for the first month.
	 */
	virtual ColorTheme * firstColor() const = 0;

	/**
	 * @brief The data for the first month.
	 */
	virtual QAbstractItemModel *firstMonth() const = 0;

	/**
	 * @brief The current day of the calendar month.
	 *
	 * Zero-indexed.
	 */
	virtual int currentDay() const = 0;

	/**
	 * @brief The ColorTheme for the second month.
	 */
	virtual ColorTheme * secondColor() const = 0;

	/**
	 * @brief The data for the second month.
	 */
	virtual QAbstractItemModel *secondMonth() const = 0;

Q_SIGNALS:
	/**
	 * @brief The label has changed
	 *
	 * @param label
	 */
	void labelChanged(const QString &label);

	/**
	 * @brief The username has changed
	 *
	 * @param username
	 */
	void usernameChanged(const QString &username);

	/**
	 * @brief The first month's ColorTheme has changed
	 *
	 * @param color
	 */
	void firstColorChanged(ColorTheme *color);

	/**
	 * @brief The first month's data has changed.
	 *
	 * @param firstMonth
	 *
	 * More fine-grained changed notification also occurs
	 * using the QAbstractItemModel signals.
	 */
	void firstMonthChanged(QAbstractItemModel *firstMonth);

	/**
	 * @brief The current day of the month has changed.
	 *
	 * @param currentDay
	 *
	 * Note: Zero-indexed.
	 */
	void currentDayChanged(int currentDay);

	/**
	 * @brief The second month's ColorTheme has changed
	 *
	 * @param color
	 */
	void secondColorChanged(ColorTheme *color);

	/**
	 * @brief The second month's data has changed.
	 *
	 * @param secondMonth
	 *
	 * More fine-grained changed notification also occurs
	 * using the QAbstractItemModel signals.
	 */
	void secondMonthChanged(QAbstractItemModel *secondMonth);

	/**
	 * @brief Request the current user's next data source.
	 */
	void nextDataSource();

	/**
	 * @brief Inform the UserMetrics that you are ready for data change.
	 */
	void readyForDataChange();

	/**
	 * @brief Data is about to appear.
	 *
	 * To continue, fire the #readyForDataChange signal.
	 */
	void dataAboutToAppear();

	/**
	 * @brief Data has finished loading.
	 */
	void dataAppeared();

	/**
	 * @brief Data is about to change from one set to another.
	 *
	 * To continue, fire the #readyForDataChange signal.
	 */
	void dataAboutToChange();

	/**
	 * @brief Insert documentation here.
	 */
	void dataChanged();

	/**
	 * @brief About to change to a user with no data.
	 *
	 * To continue, fire the #readyForDataChange signal.
	 */
	void dataAboutToDisappear();

	/**
	 * @brief The empty data has now been loaded.
	 */
	void dataDisappeared();

public Q_SLOTS:
	/**
	 * @brief Synchronous version of #nextDataSource
	 */
	virtual void nextDataSourceSlot() = 0;

	/**
	 * @brief Synchronous version of #readyForDataChange
	 */
	virtual void readyForDataChangeSlot() = 0;

protected:
	/**
	 * @brief Unusable constructor - this class is pure-virtual.
	 *
	 * @param parent
	 */
	explicit UserMetrics(QObject *parent = 0);

	Q_DISABLE_COPY(UserMetrics)

};

}

/*@}*/

#endif // USERMETRICSOUTPUT_USERMETRICS_H_
