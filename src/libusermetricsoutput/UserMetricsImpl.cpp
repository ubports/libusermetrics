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

#include <libusermetricsoutput/UserMetricsImpl.h>

#include <QtCore/QDate>
#include <QtCore/QString>
#include <QMultiMap>

using namespace UserMetricsOutput;

UserMetricsImpl::UserMetricsImpl(QObject *parent) :
		UserMetrics(parent), m_firstColor(new ColorThemeImpl(this)), m_firstMonth(
				new QVariantListModel(this)), m_secondColor(
				new ColorThemeImpl(this)), m_secondMonth(
				new QVariantListModel(this)), m_currentDay(0) {
	m_dataSets.insert("", DataSetPtr(new DataSet(this)));
	setUsername("");

	connect(this, SIGNAL(nextDataSource()), this, SLOT(nextDataSourceSlot()),
			Qt::QueuedConnection);
	connect(this, SIGNAL(readyForDataChange()), this, SLOT(
			readyForDataChangeSlot()), Qt::QueuedConnection);
}

UserMetricsImpl::~UserMetricsImpl() {
}

void UserMetricsImpl::setUsername(const QString &username) {
	if (m_username == username) {
		return;
	}

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

void UserMetricsImpl::finishLoadingDataSource() {
	bool oldLabelEmpty = m_label.isEmpty();
	bool newLabelEmpty = m_newData->formatString().isEmpty();

	m_label = m_newData->formatString();
	m_firstColor->setColors(m_newData->firstColor());
	m_firstMonth->setVariantList(m_newData->firstMonth());
	m_secondColor->setColors(m_newData->secondColor());
	m_secondMonth->setVariantList(m_newData->secondMonth());

	QDate currentDate(QDate::currentDate());
	int currentDay(currentDate.day());
	bool currentDayChanged = m_currentDay != currentDay;
	m_currentDay = currentDay;

	labelChanged(m_label);
	if (currentDayChanged) {
		this->currentDayChanged(m_currentDay);
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

