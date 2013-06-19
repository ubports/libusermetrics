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

#include <libusermetricsinput/Metric.h>
#include <libusermetricsinput/MetricUpdateImpl.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;
using namespace UserMetricsInput;

namespace {

class MockMetric: public Metric {
public:
	virtual MetricUpdatePtr update(const std::string &username) {
		return MetricUpdatePtr(updateProxy(username));
	}

	MOCK_METHOD1(updateProxy, MetricUpdate*(const std::string &));
};

class TestMetricUpdateImpl: public Test {
protected:
	TestMetricUpdateImpl() {
	}

	virtual ~TestMetricUpdateImpl() {
	}
};

TEST_F(TestMetricUpdateImpl, Foo) {
	NiceMock<MockMetric> metric;

	MetricUpdatePtr update(new MetricUpdateImpl(metric, "username"));
	update->addData(1.0);
	update->addNull();
	update->addData(0.1);
}

} // namespace