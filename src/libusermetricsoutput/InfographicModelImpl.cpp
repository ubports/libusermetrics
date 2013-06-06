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

#include <libusermetricsoutput/InfographicModelImpl.h>

#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QMultiMap>

using namespace UserMetricsOutput;

InfographicModelImpl::InfographicModelImpl(QObject *parent) :
		InfographicModel(parent), m_firstColor(
				new InfographicColorThemeImpl(this)), m_firstMonth(
				new QVariantListModel(this)), m_secondColor(
				new InfographicColorThemeImpl(this)), m_secondMonth(
				new QVariantListModel(this)), m_currentDay(0) {
	m_fakeData.insert("", InfographicDataPtr(new InfographicData(this)));
	generateFakeData();
	setUsername("");

	connect(this, SIGNAL(nextDataSource()), this, SLOT(nextDataSourceSlot()),
			Qt::QueuedConnection);
	connect(this, SIGNAL(readyForDataChange()), this, SLOT(
			readyForDataChangeSlot()), Qt::QueuedConnection);
}

InfographicModelImpl::~InfographicModelImpl() {
}

void InfographicModelImpl::setUsername(const QString &username) {
	if (m_username == username) {
		return;
	}

	m_username = username;

	m_dataIndex = m_fakeData.constFind(m_username);
	if (m_dataIndex == m_fakeData.end()) {
		m_dataIndex = m_fakeData.constFind("");
	}

	loadFakeData();

	usernameChanged(m_username);
}

void InfographicModelImpl::loadFakeData() {
	m_newData = *m_dataIndex;

	bool oldLabelEmpty = m_label.isEmpty();
	bool newLabelEmpty = m_newData->label().isEmpty();

	if (oldLabelEmpty && !newLabelEmpty) {
		dataAboutToAppear();
		finishSetFakeData();
	} else if (!oldLabelEmpty && newLabelEmpty) {
		dataAboutToDisappear();
	} else if (!oldLabelEmpty && !newLabelEmpty) {
		dataAboutToChange();
	}
// we emit no signal if the data has stayed empty
}

void InfographicModelImpl::finishSetFakeData() {
	bool oldLabelEmpty = m_label.isEmpty();
	bool newLabelEmpty = m_newData->label().isEmpty();

	m_label = m_newData->label();
	m_firstColor->setColors(m_newData->firstColor());
	m_firstMonth->setVariantList(m_newData->firstMonth());
	m_secondColor->setColors(m_newData->secondColor());
	m_secondMonth->setVariantList(m_newData->secondMonth());

	bool currentDayChanged = m_currentDay != m_newData->length();
	m_currentDay = m_newData->length();

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

void InfographicModelImpl::nextFakeData() {
	++m_dataIndex;
	if (m_dataIndex == m_fakeData.end() || m_dataIndex.key() != m_username) {
		m_dataIndex = m_fakeData.constFind(m_username);
	}

	loadFakeData();
}

QString InfographicModelImpl::label() const {
	return m_label;
}

QString InfographicModelImpl::username() const {
	return m_username;
}

InfographicColorTheme * InfographicModelImpl::firstColor() const {
	return m_firstColor.data();
}

InfographicColorTheme * InfographicModelImpl::secondColor() const {
	return m_secondColor.data();
}

QAbstractItemModel * InfographicModelImpl::firstMonth() const {
	return m_firstMonth.data();
}

QAbstractItemModel * InfographicModelImpl::secondMonth() const {
	return m_secondMonth.data();
}

int InfographicModelImpl::currentDay() const {
	return m_currentDay;
}

void InfographicModelImpl::nextDataSourceSlot() {
	nextFakeData();
}

void InfographicModelImpl::readyForDataChangeSlot() {
	finishSetFakeData();
}

