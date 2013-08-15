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

#include <libusermetricsoutput/UserMetrics.h>

#include <testutils/DBusTest.h>

#include <gtest/gtest.h>

using namespace std;
using namespace UserMetricsOutput;
using namespace UserMetricsTestUtils;

namespace {

class TestUserMetrics: public DBusTest {
protected:
	TestUserMetrics() {
	}

	virtual ~TestUserMetrics() {
	}
};

TEST_F(TestUserMetrics, MethodBarDoesAbc) {
	QScopedPointer<UserMetrics> model(UserMetrics::getInstance());
	model->setUsername("guest");
	EXPECT_EQ("guest", model->username());
}

}  // namespace
