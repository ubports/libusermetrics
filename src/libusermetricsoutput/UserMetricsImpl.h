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

#include <libusermetricsoutput/UserMetrics.h>
#include <libusermetricsoutput/UserMetricsStore.h>
#include <libusermetricscommon/DateFactory.h>
#include <libusermetricsoutput/ColorThemeImpl.h>
#include <libusermetricsoutput/ColorThemeProvider.h>
#include <libusermetricsoutput/qvariantlistmodel.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QScopedPointer>

namespace UserMetricsOutput {

class UserMetricsImpl: public UserMetrics {
Q_OBJECT

public:
	UserMetricsImpl(QSharedPointer<UserMetricsCommon::DateFactory> dateFactory,
			QSharedPointer<UserMetricsStore> dataSetStore,
			QSharedPointer<ColorThemeProvider> colorThemeProvider,
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

public Q_SLOTS:
	virtual void nextDataSourceSlot();

	virtual void readyForDataChangeSlot();

protected Q_SLOTS:
	virtual void updateCurrentDataSet(const QVariantList *data);

	virtual void userDataAdded(const QString &username, UserDataPtr userData);

	virtual void dataSetAdded(const QString &dataSourceName);

protected:
	virtual void prepareToLoadDataSource();

	virtual void finishLoadingDataSource();

	virtual void setUsernameInternal(const QString &username);

	virtual void updateMonth(QVariantListModel &month, const int dayOfMonth,
			const int daysInMonth, QVariantList::const_iterator& index,
			const QVariantList::const_iterator& end);

	virtual void checkForUserData();

	QSharedPointer<UserMetricsCommon::DateFactory> m_dateFactory;

	QSharedPointer<UserMetricsStore> m_userMetricsStore;

	QSharedPointer<ColorThemeProvider> m_colorThemeProvider;

	QString m_label;

	QScopedPointer<ColorThemeImpl> m_firstColor;

	QScopedPointer<QVariantListModel> m_firstMonth;

	QScopedPointer<ColorThemeImpl> m_secondColor;

	QScopedPointer<QVariantListModel> m_secondMonth;

	int m_currentDay;

	bool m_noDataForUser;

	bool m_oldNoDataForUser;

	QString m_username;

	QString m_watchUser;

	UserMetricsStore::const_iterator m_userDataIterator;

	UserDataPtr m_userData;

	UserData::const_iterator m_dataSetIterator;

	DataSetPtr m_dataSet;
};

}

#endif /* USERMETRICSOUTPUT_USERMETRICSIMPL_H_ */
