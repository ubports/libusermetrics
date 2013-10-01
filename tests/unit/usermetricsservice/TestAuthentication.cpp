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

#include <stdexcept>

#include <usermetricsservice/Authentication.h>

#include <gtest/gtest.h>

using namespace testing;
using namespace UserMetricsService;

namespace {

class TestAuthentication: public Test {
protected:
	TestAuthentication() {
	}

	virtual ~TestAuthentication() {
	}

	void checkCanoncialize(const QString &expected,
			const QString &confinementContext) {
		QString input(confinementContext);
		auth.canonicalizeConfinementContext(input);
		EXPECT_EQ(expected, input);
	}

	Authentication auth;
};

TEST_F(TestAuthentication, CanonicalizesContexts) {
	checkCanoncialize("com.ubuntu.dropping-letters",
			"com.ubuntu.dropping-letters_dropping-letters_0.1.2.2");
}

TEST_F(TestAuthentication, IgnoresFullPaths) {
	checkCanoncialize("/foo/bar/baz", "/foo/bar/baz");
}

TEST_F(TestAuthentication, IgnoresNormalNames) {
	checkCanoncialize("my_cool_app", "my_cool_app");
}

} // namespace

