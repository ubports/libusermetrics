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

#include <libusermetricsinput/MetricManager.h>
#include <testutils/DBusTest.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;
using namespace UserMetricsInput;
using namespace UserMetricsTestUtils;

namespace {

class TestMetricManager: public DBusTest {
protected:
	TestMetricManager() {
	}

	virtual ~TestMetricManager() {
	}
};

TEST_F(TestMetricManager, Basic) {
	MetricManagerPtr manager(MetricManager::getInstance());

	MetricPtr metric(manager->add("data-source-id", "format string %1"));

	MetricUpdatePtr update(metric->update("username"));
	update->addData(1.0);
	update->addNull();
	update->addData(0.5);
}

} // namespace
