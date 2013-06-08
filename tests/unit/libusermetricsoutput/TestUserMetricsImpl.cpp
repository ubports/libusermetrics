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
#include <gtest/gtest.h>

using namespace std;
using namespace UserMetricsOutput;

namespace {

// The fixture for testing class Foo.
class FooTest: public ::testing::Test {
protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	FooTest() {
		// You can do set-up work for each test here.
	}

	virtual ~FooTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(FooTest, MethodBarDoesAbc) {
	QScopedPointer<UserMetrics> model(new UserMetricsImpl());
	model->setUsername("guest");
	EXPECT_EQ("guest", model->username());
	QAbstractItemModel *firstMonth(model->firstMonth());
	EXPECT_EQ(31, firstMonth->rowCount());
}

// Tests that Foo does Xyz.
TEST_F(FooTest, DoesXyz) {
	// Exercises the Xyz feature of Foo.
}

}  // namespace
