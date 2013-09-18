/*
 * Copyright 2013 Canonical Ltd.
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
 */

import QtQuick 2.0
import QtTest 1.0
import "../../"
import "../../.." //Needed for out of source build
import UserMetrics 0.1
import UserMetricsTest 0.1

TestCase {
    name: "StopWatch"

    function test_time_format_calc() {
        //compare(stopWatch.elapsed, "00:05", "Time not calculated correctly")
        return true;
    }

    Metric {
        id: metric
        name: "test"
        format: "Test metric"
    }
}
