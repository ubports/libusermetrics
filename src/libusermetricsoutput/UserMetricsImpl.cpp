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
#include <QtCore/QMultiMap>

using namespace UserMetricsOutput;

UserMetricsImpl::UserMetricsImpl(QSharedPointer<DateFactory> dateFactory,
		QObject *parent) :
		UserMetrics(parent), m_dateFactory(dateFactory), m_firstColor(
				new ColorThemeImpl(this)), m_firstMonth(
				new QVariantListModel(this)), m_secondColor(
				new ColorThemeImpl(this)), m_secondMonth(
				new QVariantListModel(this)), m_currentDay(0) {
	connect(this, SIGNAL(nextDataSource()), this, SLOT(nextDataSourceSlot()),
			Qt::QueuedConnection);
	connect(this, SIGNAL(readyForDataChange()), this, SLOT(
			readyForDataChangeSlot()), Qt::QueuedConnection);

	DataSetPtr emptyData(new DataSet(ColorThemeImpl(), ColorThemeImpl(), this));
	emptyData->setFormatString("No data sources available");
	m_dataSets.insert("", emptyData);

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

	m_dataIndex = m_dataSets.constFind(m_username);
	if (m_dataIndex == m_dataSets.end()) {
		m_dataIndex = m_dataSets.constFind("");
	}

	prepareToLoadDataSource();

	usernameChanged(m_username);
}

void UserMetricsImpl::prepareToLoadDataSource() {
	m_newData = *m_dataIndex;

	bool oldLabelEmpty = m_label.isEmpty();
	bool newLabelEmpty = m_newData->formatString().isEmpty();

	if (oldLabelEmpty && !newLabelEmpty) {
		dataAboutToAppear();
		finishLoadingDataSource();
	} else if (!oldLabelEmpty && newLabelEmpty) {
		dataAboutToDisappear();
	} else if (!oldLabelEmpty && !newLabelEmpty) {
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
	bool oldLabelEmpty = m_label.isEmpty();
	bool newLabelEmpty = m_newData->formatString().isEmpty();

	const QDate currentDate(m_dateFactory->currentDate());
	const QDate &lastUpdated(m_newData->lastUpdated());
	QDate secondMonthDate(currentDate.addMonths(-1));

	int valuesToCopyForFirstMonth(0);
	int valuesToCopyForSecondMonth(0);

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

	setCurrentDay(currentDate.day() - 1);

	m_firstColor->setColors(m_newData->firstColor());
	m_secondColor->setColors(m_newData->secondColor());

	const QVariantList &newData = m_newData->data();
	QVariantList::const_iterator dataIndex(newData.begin());
	QVariantList::const_iterator end(m_newData->data().end());

	updateMonth(*m_firstMonth, valuesToCopyForFirstMonth,
			currentDate.daysInMonth(), dataIndex, end);

	updateMonth(*m_secondMonth, valuesToCopyForSecondMonth,
			secondMonthDate.daysInMonth(), dataIndex, end);

	if (newData.empty() || valuesToCopyForFirstMonth == 0) {
		setLabel("No data for today");
	} else {
		setLabel(m_newData->formatString().arg(newData.first().toString()));
	}

	if (oldLabelEmpty && !newLabelEmpty) {
		dataAppeared();
	} else if (!oldLabelEmpty && newLabelEmpty) {
		dataDisappeared();
	} else if (!oldLabelEmpty && !newLabelEmpty) {
		dataChanged();
	}
	// we emit no signal if the data has stayed empty
}

UserMetricsImpl::DataSetPtr & UserMetricsImpl::data(const QString &username,
		const QString &dataSourceId) {
	DataSetMap::iterator data(m_dataSets.find(username));

	if (data == m_dataSets.end()) {
		DataSetPtr newData(
				new DataSet(ColorThemeImpl(), ColorThemeImpl(), this));
		data = m_dataSets.insert(username, newData);
	}

	return *data;
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
	++m_dataIndex;
	if (m_dataIndex == m_dataSets.end() || m_dataIndex.key() != m_username) {
		m_dataIndex = m_dataSets.constFind(m_username);
	}

	prepareToLoadDataSource();
}

void UserMetricsImpl::readyForDataChangeSlot() {
	finishLoadingDataSource();
}

