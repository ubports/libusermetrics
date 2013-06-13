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
#include <utils/QVariantPrinter.h>
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
				Return(QDate(2001, 01, 07)));

		userDataStore.reset(new UserDataStore());

		model.reset(new UserMetricsImpl(dateFactory, userDataStore));
	}

	virtual ~UserMetricsImplTest() {
	}

	QSharedPointer<MockDateFactory> dateFactory;

	QSharedPointer<UserDataStore> userDataStore;

	QScopedPointer<UserMetricsImpl> model;
}
;

TEST_F(UserMetricsImplTest, CurrentDate) {
	EXPECT_EQ(6, model->currentDay());
}

TEST_F(UserMetricsImplTest, CurrentDateChangesWithDataSource) {
	EXPECT_EQ(6, model->currentDay());

	EXPECT_CALL(*dateFactory, currentDate()).Times(2).WillOnce(
			Return(QDate(2001, 01, 21))).WillOnce(Return(QDate(2001, 01, 27)));

	StrictMock<MockSignalReceiverInt> signalReceiver;
	EXPECT_CALL(signalReceiver, receivedSignal(20)).Times(1);
	EXPECT_CALL(signalReceiver, receivedSignal(26)).Times(1);

	QObject::connect(model.data(), SIGNAL(currentDayChanged(int)),
			&signalReceiver, SLOT(receivedSignal(int)));

	model->nextDataSourceSlot();
	model->readyForDataChangeSlot();
	EXPECT_EQ(20, model->currentDay());

	model->nextDataSourceSlot();
	model->readyForDataChangeSlot();
	EXPECT_EQ(26, model->currentDay());
}

TEST_F(UserMetricsImplTest, MonthLengthChangesWithDate) {
	EXPECT_EQ(6, model->currentDay());

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
	EXPECT_EQ(QString("No data sources available"), model->label());

	// the first month should be entirely empty
	{
		const QAbstractItemModel* month(model->firstMonth());
		EXPECT_EQ(31, month->rowCount());
		for (int i(0); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}

	// the second month should be entirely empty
	{
		const QAbstractItemModel* month(model->secondMonth());
		EXPECT_EQ(31, month->rowCount());
		for (int i(0); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}
}

TEST_F(UserMetricsImplTest, HasEmptyDataForKnownUserWithNoData) {
	userDataStore->find("existing-user");

	model->setUsername("existing-user");
	model->readyForDataChangeSlot();

	EXPECT_EQ("existing-user", model->username());
	EXPECT_EQ(QString("No data sources available"), model->label());

	// the first month should be entirely empty
	{
		const QAbstractItemModel* month(model->firstMonth());
		EXPECT_EQ(31, month->rowCount());
		for (int i(0); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}

	// the second month should be entirely empty
	{
		const QAbstractItemModel* month(model->secondMonth());
		EXPECT_EQ(31, month->rowCount());
		for (int i(0); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}
}

TEST_F(UserMetricsImplTest, AddDataForToday) {
	// the fake date provider says the date is 2001/01/07

	QVariantList data;

	// First month (January) data:
	data << 100.0 << 95.0;
	while (data.size() < 5) {
		data << 0.0;
	}
	data << 90.0 << 85.0;

	// Second month (December) data:
	// December has 31 days
	data << 80.0 << 75.0;
	while (data.size() < 36) {
		data << 0.0;
	}
	data << 70.0 << 65.0;

	UserDataStore::iterator userDataIterator(userDataStore->find("username"));
	UserDataStore::UserDataPtr userData(*userDataIterator);

	UserData::iterator dataSetIterator = userData->find("data-source-id");
	UserData::DataSetPtr dataSet(*dataSetIterator);

	dataSet->setFormatString("test format string %1");

	// The data starts today
	dataSet->setData(QDate(2001, 01, 07), data);

	model->setUsername("username");
	model->readyForDataChangeSlot();

	EXPECT_EQ(QString("test format string 100"), model->label());

	// assertions about first month's data
	{
		const QAbstractItemModel* month(model->firstMonth());
		EXPECT_EQ(31, month->rowCount());
		EXPECT_EQ(QVariant(85.0), month->data(month->index(0, 0)));
		EXPECT_EQ(QVariant(90.0), month->data(month->index(1, 0)));
		for (int i(2); i < 4; ++i) {
			EXPECT_EQ(QVariant(0.0), month->data(month->index(i, 0)));
		}
		EXPECT_EQ(QVariant(95.0), month->data(month->index(5, 0)));
		EXPECT_EQ(QVariant(100.0), month->data(month->index(6, 0)));
		// the rest of the month should be padded with empty variants
		for (int i(7); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}

	// assertions about second month's data
	{
		const QAbstractItemModel* month(model->secondMonth());
		EXPECT_EQ(31, month->rowCount());
		EXPECT_EQ(QVariant(65.0), month->data(month->index(0, 0)));
		EXPECT_EQ(QVariant(70.0), month->data(month->index(1, 0)));
		for (int i(2); i < 29; ++i) {
			EXPECT_EQ(QVariant(0.0), month->data(month->index(i, 0)));
		}
		EXPECT_EQ(QVariant(75.0), month->data(month->index(29, 0)));
		EXPECT_EQ(QVariant(80.0), month->data(month->index(30, 0)));
	}
}

TEST_F(UserMetricsImplTest, AddOldDataUpdatedThisMonth) {
	// the fake date provider says the date is 2001/01/07

	QVariantList data;

	// First month (January) data:
	data << 95.0 << 100.0 << 90.0 << 85.0;

	// Second month (December) data:
	// December has 31 days
	data << 80.0 << 75.0;
	while (data.size() < 20) {
		data << 0.0;
	}
	data << 70.0 << 65.0;

	UserDataStore::iterator userDataIterator(userDataStore->find("username"));
	UserDataStore::UserDataPtr userData(*userDataIterator);

	UserData::iterator dataSetIterator = userData->find("data-source-id");
	UserData::DataSetPtr dataSet(*dataSetIterator);

	dataSet->setFormatString("test other format string %1");

	// The data starts 3 days ago
	dataSet->setData(QDate(2001, 01, 04), data);

	model->setUsername("username");
	model->readyForDataChangeSlot();

	EXPECT_EQ(QString("test other format string 95"), model->label());

	// assertions about first month's data
	{
		const QAbstractItemModel* month(model->firstMonth());
		EXPECT_EQ(31, month->rowCount());
		EXPECT_EQ(QVariant(85.0), month->data(month->index(0, 0)));
		EXPECT_EQ(QVariant(90.0), month->data(month->index(1, 0)));
		EXPECT_EQ(QVariant(100.0), month->data(month->index(2, 0)));
		EXPECT_EQ(QVariant(95.0), month->data(month->index(3, 0)));
		// the rest of the month should be padded with empty variants
		for (int i(4); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}

	// assertions about second month's data
	{
		const QAbstractItemModel* month(model->secondMonth());
		EXPECT_EQ(31, month->rowCount());
		// the start of the month should be padded with empty variants
		for (int i(0); i < 13; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
		EXPECT_EQ(QVariant(65.0), month->data(month->index(13, 0)));
		EXPECT_EQ(QVariant(70.0), month->data(month->index(14, 0)));
		for (int i(15); i < 29; ++i) {
			EXPECT_EQ(QVariant(0.0), month->data(month->index(i, 0)));
		}
		EXPECT_EQ(QVariant(75.0), month->data(month->index(29, 0)));
		EXPECT_EQ(QVariant(80.0), month->data(month->index(30, 0)));
	}
}

TEST_F(UserMetricsImplTest, AddOldDataUpdatedLastMonth) {
	// the fake date provider says the date is 2001/01/07

	QVariantList data;

	// Data just for December
	data << 95.0 << 100.0 << 90.0 << 0.0;

	UserDataStore::iterator userDataIterator(userDataStore->find("username"));
	UserDataStore::UserDataPtr userData(*userDataIterator);

	UserData::iterator dataSetIterator = userData->find("data-source-id");
	UserData::DataSetPtr dataSet(*dataSetIterator);

	dataSet->setFormatString("this format string won't be used %1");

	// The data starts 3 days ago
	dataSet->setData(QDate(2000, 12, 25), data);

	model->setUsername("username");
	model->readyForDataChangeSlot();

	EXPECT_EQ(QString("No data for today"), model->label());

	// assertions about first month's data
	{
		const QAbstractItemModel* month(model->firstMonth());
		EXPECT_EQ(31, month->rowCount());
		// the whole month should be padded with empty variants
		for (int i(0); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}

	// assertions about second month's data
	{
		const QAbstractItemModel* month(model->secondMonth());
		EXPECT_EQ(31, month->rowCount());
		// the start of the month should be padded with empty variants
		for (int i(0); i < 21; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
		EXPECT_EQ(QVariant(0.0), month->data(month->index(21, 0)));
		EXPECT_EQ(QVariant(90.0), month->data(month->index(22, 0)));
		EXPECT_EQ(QVariant(100.0), month->data(month->index(23, 0)));
		EXPECT_EQ(QVariant(95.0), month->data(month->index(24, 0)));
		for (int i(25); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}
}

TEST_F(UserMetricsImplTest, AddDataUpdatedThisMonthButNotEnoughToFillTheMonth) {
	// the fake date provider says the date is 2001/01/07

	QVariantList data;

	// Data just for January
	data << 100.0 << 0.0;

	UserDataStore::iterator userDataIterator(userDataStore->find("username"));
	UserDataStore::UserDataPtr userData(*userDataIterator);

	UserData::iterator dataSetIterator = userData->find("data-source-id");
	UserData::DataSetPtr dataSet(*dataSetIterator);

	dataSet->setFormatString("a format string with %1 in it");

	// The data starts 2 days ago
	dataSet->setData(QDate(2001, 1, 5), data);

	model->setUsername("username");
	model->readyForDataChangeSlot();

	EXPECT_EQ(QString("a format string with 100 in it"), model->label());

	// assertions about first month's data
	{
		const QAbstractItemModel* month(model->firstMonth());
		EXPECT_EQ(31, month->rowCount());
		// the start of the month should be padded with empty variants
		for (int i(0); i < 3; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
		EXPECT_EQ(QVariant(0.0), month->data(month->index(3, 0)));
		EXPECT_EQ(QVariant(100.0), month->data(month->index(4, 0)));
		for (int i(5); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}

	// assertions about second month's data
	{
		const QAbstractItemModel* month(model->secondMonth());
		EXPECT_EQ(31, month->rowCount());
		// the whole month should be padded with empty variants
		for (int i(0); i < 31; ++i) {
			EXPECT_EQ(QVariant(), month->data(month->index(i, 0)));
		}
	}
}

} // namespace
