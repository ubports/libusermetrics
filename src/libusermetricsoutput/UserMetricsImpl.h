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

#ifndef USERMETRICSOUTPUT_USERMETRICSIMPL_H_
#define USERMETRICSOUTPUT_USERMETRICSIMPL_H_

#include <libusermetricsoutput/DataSet.h>
#include <libusermetricsoutput/DateFactory.h>
#include <libusermetricsoutput/UserMetrics.h>
#include <libusermetricsoutput/qvariantlistmodel.h>

#include <QtCore/QScopedPointer>

namespace UserMetricsOutput {

class UserMetricsImpl: public UserMetrics {
Q_OBJECT

public:
	typedef QSharedPointer<DataSet> DataSetPtr;
	typedef QMultiMap<QString, DataSetPtr> DataSetMap;

	UserMetricsImpl(QSharedPointer<DateFactory> dateFactory,
			QObject *parent = 0);

	virtual ~UserMetricsImpl();

	virtual void setLabel(const QString &label);

	virtual QString label() const;

	virtual QString username() const;

	virtual void setUsername(const QString &username);

	virtual ColorTheme * firstColor() const;

	virtual QAbstractItemModel *firstMonth() const;

	virtual int currentDay() const;

	virtual void setCurrentDay(int currentDay);

	virtual ColorTheme * secondColor() const;

	virtual QAbstractItemModel *secondMonth() const;

	virtual DataSetPtr & data(const QString &username,
			const QString &dataSourceId);

public Q_SLOTS:
	virtual void nextDataSourceSlot();

	virtual void readyForDataChangeSlot();

protected:
	virtual void prepareToLoadDataSource();

	virtual void finishLoadingDataSource();

	virtual void setUsernameInternal(const QString &username);

	void updateMonth(QVariantListModel &month, const int dayOfMonth,
			const int daysInMonth, QVariantList::const_iterator& index,
			const QVariantList::const_iterator& end);

	QSharedPointer<DateFactory> m_dateFactory;

	QString m_label;

	QScopedPointer<ColorThemeImpl> m_firstColor;

	QScopedPointer<QVariantListModel> m_firstMonth;

	QScopedPointer<ColorThemeImpl> m_secondColor;

	QScopedPointer<QVariantListModel> m_secondMonth;

	int m_currentDay;

	QString m_username;

	DataSetMap::const_iterator m_dataIndex;

	DataSetPtr m_newData;

	DataSetMap m_dataSets;
};

}

#endif /* USERMETRICSOUTPUT_USERMETRICSIMPL_H_ */
