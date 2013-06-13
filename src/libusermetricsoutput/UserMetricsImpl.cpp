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

#include <libusermetricsoutput/UserMetricsImpl.h>

#include <QtCore/QDate>
#include <QtCore/QString>
#include <QtCore/QVariantList>

using namespace UserMetricsOutput;

UserMetricsImpl::UserMetricsImpl(QSharedPointer<DateFactory> dateFactory,
		QSharedPointer<UserDataStore> userDataStore, QObject *parent) :
		UserMetrics(parent), m_dateFactory(dateFactory), m_userDataStore(
				userDataStore), m_firstColor(new ColorThemeImpl(this)), m_firstMonth(
				new QVariantListModel(this)), m_secondColor(
				new ColorThemeImpl(this)), m_secondMonth(
				new QVariantListModel(this)), m_currentDay(), m_noDataForUser(
				false), m_oldNoDataForUser(false) {
	connect(this, SIGNAL(nextDataSource()), this, SLOT(nextDataSourceSlot()),
			Qt::QueuedConnection);
	connect(this, SIGNAL(readyForDataChange()), this, SLOT(
			readyForDataChangeSlot()), Qt::QueuedConnection);

	setCurrentDay(m_dateFactory->currentDate().day() - 1);
	setUsernameInternal("");
}

UserMetricsImpl::~UserMetricsImpl() {
}

void UserMetricsImpl::setLabel(const QString &label) {
	bool labelChanged = m_label != label;
	m_label = label;
	if (labelChanged) {
		this->labelChanged(m_label);
	}
}

void UserMetricsImpl::setCurrentDay(int currentDay) {
	bool currentDayChanged = m_currentDay != currentDay;
	m_currentDay = currentDay;
	if (currentDayChanged) {
		this->currentDayChanged(m_currentDay);
	}
}

void UserMetricsImpl::setUsername(const QString &username) {
	if (m_username == username) {
		return;
	}

	setUsernameInternal(username);
}

void UserMetricsImpl::setUsernameInternal(const QString &username) {
	m_username = username;

	m_oldNoDataForUser = m_noDataForUser;

	m_userDataIterator = m_userDataStore->constFind(m_username);

	// first check to see if there is UserData for this user
	m_noDataForUser = m_userDataIterator == m_userDataStore->constEnd();
	if (!m_noDataForUser) {
		// if there is a UserData container
		m_userData = *m_userDataIterator;
		m_dataSetIterator = m_userData->constBegin();

		// now check to see if that container has any data in
		m_noDataForUser = m_dataSetIterator == m_userData->constEnd();
		if (!m_noDataForUser) {
			m_dataSet = *m_dataSetIterator;
		}
	}

	prepareToLoadDataSource();

	usernameChanged(m_username);
}

void UserMetricsImpl::prepareToLoadDataSource() {
	if (m_oldNoDataForUser && !m_noDataForUser) {
		dataAboutToAppear();
		finishLoadingDataSource();
	} else if (!m_oldNoDataForUser && m_noDataForUser) {
		dataAboutToDisappear();
	} else if (!m_oldNoDataForUser && !m_noDataForUser) {
		dataAboutToChange();
	}
// we emit no signal if the data has stayed empty
}

void UserMetricsImpl::updateMonth(QVariantListModel &month,
		const int dayOfMonth, const int daysInMonth,
		QVariantList::const_iterator& index,
		const QVariantList::const_iterator& end) {

	// Data for the month
	QVariantList newData;

	// Copy a number of data entries equal to the day of the month it is
	for (int i(0); i < dayOfMonth; ++i) {
		if (index == end) {
			// when we run out of data, pad the remaining days to the
			// start of the month
			newData.prepend(QVariant());
		} else {
			// pop data from the from of the source
			newData.prepend(*index);
			++index;
		}
	}

	// Now fill the end of the month with empty data
	while (newData.size() < daysInMonth) {
		newData.append(QVariant());
	}

	month.setVariantList(newData);
}

void UserMetricsImpl::finishLoadingDataSource() {
	const QDate currentDate(m_dateFactory->currentDate());

	const QDate &lastUpdated(m_dataSet->lastUpdated());
	QDate secondMonthDate(currentDate.addMonths(-1));

	int valuesToCopyForFirstMonth(0);
	int valuesToCopyForSecondMonth(0);

	if (m_noDataForUser) {
		QVariantList data;
		QVariantList::const_iterator dataIndex(data.begin());
		QVariantList::const_iterator end(data.end());

		updateMonth(*m_firstMonth, valuesToCopyForFirstMonth,
				currentDate.daysInMonth(), dataIndex, end);

		updateMonth(*m_secondMonth, valuesToCopyForSecondMonth,
				secondMonthDate.daysInMonth(), dataIndex, end);

		setLabel("No data sources available");
	} else {
		if (currentDate.year() == lastUpdated.year()
				&& currentDate.month() == lastUpdated.month()) {
			// If the data is for the first month
			valuesToCopyForFirstMonth = lastUpdated.day();
			valuesToCopyForSecondMonth = secondMonthDate.daysInMonth();
		} else if (secondMonthDate.year() == lastUpdated.year()
				&& secondMonthDate.month() == lastUpdated.month()) {
			// If the data is for the second month
			valuesToCopyForSecondMonth = lastUpdated.day();
		} else {
			// the data is out of date
		}

		m_firstColor->setColors(m_dataSet->firstColor());
		m_secondColor->setColors(m_dataSet->secondColor());

		const QVariantList &data(m_dataSet->data());

		QVariantList::const_iterator dataIndex(data.begin());
		QVariantList::const_iterator end(data.end());

		updateMonth(*m_firstMonth, valuesToCopyForFirstMonth,
				currentDate.daysInMonth(), dataIndex, end);

		updateMonth(*m_secondMonth, valuesToCopyForSecondMonth,
				secondMonthDate.daysInMonth(), dataIndex, end);

		if (data.empty() || currentDate != lastUpdated) {
			setLabel("No data for today");
		} else {
			setLabel(m_dataSet->formatString().arg(data.first().toString()));
		}
	}

	setCurrentDay(currentDate.day() - 1);

	if (m_oldNoDataForUser && !m_noDataForUser) {
		dataAppeared();
	} else if (!m_oldNoDataForUser && m_noDataForUser) {
		dataDisappeared();
	} else if (!m_oldNoDataForUser && !m_noDataForUser) {
		dataChanged();
	}
	// we emit no signal if the data has stayed empty
}

QString UserMetricsImpl::label() const {
	return m_label;
}

QString UserMetricsImpl::username() const {
	return m_username;
}

ColorTheme * UserMetricsImpl::firstColor() const {
	return m_firstColor.data();
}

ColorTheme * UserMetricsImpl::secondColor() const {
	return m_secondColor.data();
}

QAbstractItemModel * UserMetricsImpl::firstMonth() const {
	return m_firstMonth.data();
}

QAbstractItemModel * UserMetricsImpl::secondMonth() const {
	return m_secondMonth.data();
}

int UserMetricsImpl::currentDay() const {
	return m_currentDay;
}

void UserMetricsImpl::nextDataSourceSlot() {
	if (m_noDataForUser) {
	} else {
		++m_dataSetIterator;
		if (m_dataSetIterator == m_userData->constEnd()) {
			m_dataSetIterator = m_userData->constBegin();
		}
		m_dataSet = *m_dataSetIterator;
	}

	prepareToLoadDataSource();
}

void UserMetricsImpl::readyForDataChangeSlot() {
	finishLoadingDataSource();
}

