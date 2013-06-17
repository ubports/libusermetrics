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

#include <libusermetricsinput/MetricUpdate.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;
using namespace UserMetricsInput;

namespace {

class TestDataChange: public Test {
protected:
	TestDataChange() {
	}

	virtual ~TestDataChange() {
	}
};

TEST_F(TestDataChange, Foo) {
	MetricUpdatePtr metricUpdate(
			MetricUpdate::getInstance("dataSourceId", "username"));
	metricUpdate->addData(1.0);
	metricUpdate->addNull();
	metricUpdate->addData(0.1);
}

} // namespace
