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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace UserMetricsOutput;
using namespace testing;

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

TEST_F(UserMetricsImplTest, HasEmptyDataForNonExistentUser) {
	model->setUsername("non-existing-user");
	model->readyForDataChange();

	EXPECT_EQ("non-existing-user", model->username());
	EXPECT_EQ(QString("No data"), model->label());
}

}// namespace