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
import UserMetrics 0.1
import UserMetricsTest 0.1

TestCase {
    name: "Increment"
    property string originalName: "test"
    property string originalFormat: "Test Metric"
    property string originalEmptyFormat: "Nope, no data"
    property string originalDomain: "test-domain"
    property double originalMinimum: 0.0

    Metric {
        id: metric
        name: originalName
        format: originalFormat
        emptyFormat: originalEmptyFormat
        domain: originalDomain
        minimum: originalMinimum
    }

    function test_metric() {
        // Make sure the metric properties have been set properly
        var info = dbusQuery.queryMetricInfo(1);
        compare(info.name, originalName, "Metric name was not set properly");
        compare(info.format, originalFormat, "Metric format was not set properly");
        compare(info.emptyFormat, originalEmptyFormat, "Metric emptyFormat was not set properly");
        compare(info.domain, originalDomain, "Metric domain was not set properly");
        compare(info.minimum, originalMinimum, "Metric minimum was not set properly");
        compare(info.maximum, NaN, "Metric maximum was not set properly");
        
        metric.minimum = 0.0
        metric.maximum = 10.0
        info = dbusQuery.queryMetricInfo(1);
        compare(info.minimum, 0.0, "Metric minimum was not set properly");
        compare(info.maximum, 10.0, "Metric maximum was not set properly");

        // Test update and increment with integers and floating point data
        metric.update(0);
        compare(dbusQuery.queryCurrentValue(1), 0, "Data not updated successfully")
        metric.increment();
        compare(dbusQuery.queryCurrentValue(1), 1, "Data not incremented successfully")
        metric.increment(7776);
        compare(dbusQuery.queryCurrentValue(1), 7777, "Data not incremented successfully")
        metric.increment(0.999);
        compare(dbusQuery.queryCurrentValue(1), 7777.999, "Data not incremented successfully")
        metric.update(5.5);
        compare(dbusQuery.queryCurrentValue(1), 5.5, "Data not updated successfully")

        // Check that when changing the metric format or emptyFormat nothing else changes and
        // no new metrics are created
        var newFormat = "Something else";
        var newEmptyFormat = "Still no data";
        var newDomain = "test-new-domain";
        metric.format = newFormat;
        metric.emptyFormat = newEmptyFormat;
        metric.domain = newDomain;
        info = dbusQuery.queryMetricInfo(1);
        compare(info.name, originalName, "Metric name was changed without requesting it");
        compare(info.format, newFormat, "Metric format was not changed properly");
        compare(info.emptyFormat, newEmptyFormat, "Metric emptyFormat was not changed properly");
        compare(info.domain, newDomain, "Metric domain was not changed properly");
        info = dbusQuery.queryMetricInfo(2);
        compare(info, null, "A new metric shouldn't have been created when just changing format");

        // Check that when changing the metric name a new metric is created with the new name
        var newName = "another_test";
        metric.name = newName;
        info = dbusQuery.queryMetricInfo(1);
        compare(info.name, originalName, "Metric 1 name was changed instead of creating new metric");
        info = dbusQuery.queryMetricInfo(2);
        compare(info.name, newName, "Metric 2 was not created with proper name");
        compare(info.format, newFormat, "Metric 2 was not created with proper format");

        // Check that now the "metric" object points to the newly created metric and changes to it don't
        // affect the old metric
        metric.update(0);
        compare(dbusQuery.queryCurrentValue(1), 5.5, "Metric 1 data was changed while metric 2 was in use");
        compare(dbusQuery.queryCurrentValue(2), 0, "Metric 2 data was not updated correctly");
        metric.increment(1.5);
        compare(dbusQuery.queryCurrentValue(1), 5.5, "Metric 1 data was changed while metric 2 was in use");
        compare(dbusQuery.queryCurrentValue(2), 1.5, "Metric 2 data was not incremented correctly");

        // Make sure that making the "metric" object point back to the original metric by restoring the
        // original name works fine and has no side effects.
        metric.name = originalName;
        metric.update(0);
        compare(dbusQuery.queryCurrentValue(1), 0, "Metric 1 data was not changed when pointing to it");
        compare(dbusQuery.queryCurrentValue(2), 1.5, "Metric 2 data was changed when pointing to metric 1");
        info = dbusQuery.queryMetricInfo(3);
        compare(info, null, "A new metric shouldn't have been created when pointing back to metric 1");
    }
}
