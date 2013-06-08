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

using namespace std;
using namespace UserMetricsOutput;

namespace {

class UserMetricsImplTest: public ::testing::Test {
protected:

	UserMetricsImplTest() {
	}

	virtual ~UserMetricsImplTest() {
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};

TEST_F(UserMetricsImplTest, HasEmptyDataForNonExistentUser) {
	QScopedPointer<UserMetricsImpl> model(new UserMetricsImpl());

	model->setUsername("non-existing-user");
	EXPECT_EQ("non-existing-user", model->username());

	EXPECT_EQ(QString("No data"), model->label());
}

}  // namespace
