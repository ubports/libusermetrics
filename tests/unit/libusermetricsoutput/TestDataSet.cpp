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

#include <libusermetricsoutput/DataSet.h>

#include <testutils/QStringPrinter.h>
#include <testutils/QVariantPrinter.h>
#include <testutils/QVariantListPrinter.h>

#include <gtest/gtest.h>

using namespace std;
using namespace testing;
using namespace UserMetricsOutput;

namespace {

class TestData {
public:
	TestData(const QVariantList &input, const QVariantList &expectedData,
			const QVariant &expectedHead) :
			input(input), expectedData(expectedData), expectedHead(expectedHead) {
	}

	virtual ~TestData() {
	}

	QVariantList input;
	QVariantList expectedData;
	QVariant expectedHead;
};

class TestDataSet: public TestWithParam<TestData> {
protected:
	TestDataSet() {
	}

	virtual ~TestDataSet() {
	}
};

TEST_P(TestDataSet, SetData) {
	TestData testData(GetParam());

	DataSet dataSet;
	dataSet.setData(testData.input);

	EXPECT_EQ(testData.expectedData, dataSet.data());
	EXPECT_EQ(testData.expectedHead, dataSet.head());
}

INSTANTIATE_TEST_CASE_P(ScalesData, TestDataSet, Values(TestData(QVariantList( {
		-10.0, -5.0, 0.0, 5.0, 10.0 }), QVariantList( { 0.0, 0.25, 0.5, 0.75,
		1.0 }), QVariant(-10.0))));

INSTANTIATE_TEST_CASE_P(TurnsBlankStringIntoNull, TestDataSet,
		Values(
				TestData(QVariantList( { "" }), QVariantList( { QVariant() }),
						QVariant())));

INSTANTIATE_TEST_CASE_P(TurnsBlankStringIntoNullWithOtherData, TestDataSet,
		Values(TestData(QVariantList( { 100.0, "", 50.0, 0.0 }), QVariantList( {
				1.0, QVariant(), 0.5, 0.0 }), QVariant(100.0))));

INSTANTIATE_TEST_CASE_P(TurnsSingleValueInto0Point5, TestDataSet,
		Values(
				TestData(QVariantList( { 127.0 }), QVariantList( { 0.5 }),
						QVariant(127.0))));

INSTANTIATE_TEST_CASE_P(TurnsZeroRangeInto0Point5s, TestDataSet,
		Values(
				TestData(QVariantList( { 27.0, 27.0 }),
						QVariantList( { 0.5, 0.5 }), QVariant(27.0))));

INSTANTIATE_TEST_CASE_P(TurnsZeroRangeInto0Point5sMoreValues, TestDataSet,
		Values(TestData(QVariantList( { 150.0, 150.0, 150.0 }), QVariantList( {
				0.5, 0.5, 0.5 }), QVariant(150.0))));

}
// namespace
