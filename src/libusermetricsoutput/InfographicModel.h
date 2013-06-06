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

#ifndef USERMETRICSOUTPUT_INFOGRAPHICMODEL_H
#define USERMETRICSOUTPUT_INFOGRAPHICMODEL_H

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtCore/QAbstractItemModel>

#include <libusermetricsoutput/InfographicColorTheme.h>

namespace UserMetricsOutput {

class Q_DECL_EXPORT InfographicModel: public QObject {
Q_OBJECT

Q_PROPERTY(QString label READ label NOTIFY labelChanged FINAL)
Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
Q_PROPERTY(UserMetricsOutput::InfographicColorTheme* firstColor READ firstColor NOTIFY firstColorChanged FINAL)
Q_PROPERTY(UserMetricsOutput::InfographicColorTheme* secondColor READ secondColor NOTIFY secondColorChanged FINAL)
Q_PROPERTY(QAbstractItemModel *firstMonth READ firstMonth NOTIFY firstMonthChanged FINAL)
Q_PROPERTY(QAbstractItemModel *secondMonth READ secondMonth NOTIFY secondMonthChanged FINAL)
Q_PROPERTY(int currentDay READ currentDay NOTIFY currentDayChanged FINAL)

public:
	static InfographicModel *getInstance();

	explicit InfographicModel(QObject *parent = 0);

	virtual ~InfographicModel();

	virtual QString label() const = 0;

	virtual QString username() const = 0;

	virtual void setUsername(const QString &username) = 0;

	virtual InfographicColorTheme * firstColor() const = 0;

	virtual QAbstractItemModel *firstMonth() const = 0;

	virtual int currentDay() const = 0;

	virtual InfographicColorTheme * secondColor() const = 0;

	virtual QAbstractItemModel *secondMonth() const = 0;

Q_SIGNALS:
	void labelChanged(const QString &label);

	void usernameChanged(const QString &username);

	void firstColorChanged(InfographicColorTheme *color);

	void firstMonthChanged(QAbstractItemModel *firstMonth);

	void currentDayChanged(int length);

	void secondColorChanged(InfographicColorTheme *color);

	void secondMonthChanged(QAbstractItemModel *secondMonth);

	void nextDataSource();

	void readyForDataChange();

	void dataAboutToAppear();

	void dataAppeared();

	void dataAboutToChange();

	void dataChanged();

	void dataAboutToDisappear();

	void dataDisappeared();

protected Q_SLOTS:
	virtual void nextDataSourceSlot() = 0;

	virtual void readyForDataChangeSlot() = 0;

protected:
	Q_DISABLE_COPY(InfographicModel)

};

}

#endif // USERMETRICSOUTPUT_INFOGRAPHICMODEL_H
