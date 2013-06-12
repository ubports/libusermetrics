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
#include <utils/QStringPrinter.h>
#include <utils/MockSignalReceiver.h>

#include <QtCore/QObject>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace UserMetricsOutput;
using namespace testing;
using namespace testutils;

namespace {

class MockDateFactory: public DateFactory {
public:
	MOCK_CONST_METHOD0(currentDate, QDate());
};

class UserMetricsImplTest: public Test {
protected:

	UserMetricsImplTest() {
		dateFactory.reset(new NiceMock<MockDateFactory>());
		ON_CALL(*dateFactory, currentDate()).WillByDefault(
				Return(QDate(2000, 01, 07)));

		model.reset(new UserMetricsImpl(dateFactory));
	}

	virtual ~UserMetricsImplTest() {
	}

	QSharedPointer<MockDateFactory> dateFactory;

	QScopedPointer<UserMetricsImpl> model;
}
;

TEST_F(UserMetricsImplTest, CurrentDate) {
	EXPECT_EQ(7, model->currentDay());
}

TEST_F(UserMetricsImplTest, CurrentDateChangesWithDataSource) {
	EXPECT_EQ(7, model->currentDay());

	EXPECT_CALL(*dateFactory, currentDate()).Times(2).WillOnce(
			Return(QDate(2000, 01, 21))).WillOnce(Return(QDate(2000, 01, 27)));

	StrictMock<MockSignalReceiverInt> signalReceiver;
	EXPECT_CALL(signalReceiver, receivedSignal(21)).Times(1);
	EXPECT_CALL(signalReceiver, receivedSignal(27)).Times(1);

	QObject::connect(model.data(), SIGNAL(currentDayChanged(int)),
			&signalReceiver, SLOT(receivedSignal(int)));

	model->nextDataSourceSlot();
	model->readyForDataChangeSlot();
	EXPECT_EQ(21, model->currentDay());

	model->nextDataSourceSlot();
	model->readyForDataChangeSlot();
	EXPECT_EQ(27, model->currentDay());
}

TEST_F(UserMetricsImplTest, MonthLengthChangesWithDate) {
	EXPECT_EQ(7, model->currentDay());

	EXPECT_CALL(*dateFactory, currentDate()).Times(4).WillOnce(
			Return(QDate(2001, 01, 1))).WillOnce(Return(QDate(2001, 02, 25))).WillOnce(
			Return(QDate(2001, 03, 1))).WillOnce(Return(QDate(2001, 04, 10)));

	model->nextDataSourceSlot();
	model->readyForDataChangeSlot();
	EXPECT_EQ(31, model->firstMonth()->rowCount());
	EXPECT_EQ(31, model->secondMonth()->rowCount());

	model->nextDataSourceSlot();
	model->readyForDataChangeSlot();
	EXPECT_EQ(28, model->firstMonth()->rowCount());
	EXPECT_EQ(31, model->secondMonth()->rowCount());

	model->nextDataSourceSlot();
	model->readyForDataChangeSlot();
	EXPECT_EQ(31, model->firstMonth()->rowCount());
	EXPECT_EQ(28, model->secondMonth()->rowCount());

	model->nextDataSourceSlot();
	model->readyForDataChangeSlot();
	EXPECT_EQ(30, model->firstMonth()->rowCount());
	EXPECT_EQ(31, model->secondMonth()->rowCount());
}

TEST_F(UserMetricsImplTest, HasEmptyDataForNonExistentUser) {
	model->setUsername("non-existing-user");
	model->readyForDataChangeSlot();

	EXPECT_EQ("non-existing-user", model->username());
	EXPECT_EQ(QString("No data"), model->label());
}

TEST_F(UserMetricsImplTest, AddData) {
	QVariantList data( { 100.0, 50.0, 100.0 });

	UserMetricsImpl::DataSetPtr dataSet(
			model->data("username", "data-source-id"));
	dataSet->setFormatString("test format string %d");
	dataSet->setData(QDate(2001, 01, 01), data);

	model->setUsername("username");
	model->readyForDataChangeSlot();

	// FIXME Uncomment this
//	EXPECT_EQ(QString("test format string 100.0"), model->label());
}

} // namespace
