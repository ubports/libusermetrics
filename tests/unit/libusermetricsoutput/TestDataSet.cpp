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

#include <testutils/QVariantPrinter.h>
#include <testutils/QVariantListPrinter.h>

#include <iostream>

#include <gtest/gtest.h>

using namespace std;
using namespace testing;
using namespace UserMetricsOutput;

class TestDataSetParamData {
public:
	TestDataSetParamData(const QVariantList &input,
			const QVariantList &expectedData, const QVariant &expectedHead) :
			m_input(input), m_expectedData(expectedData), m_expectedHead(
					expectedHead) {
	}

	virtual ~TestDataSetParamData() {
	}

	QVariantList m_input;
	QVariantList m_expectedData;
	QVariant m_expectedHead;
};

void PrintTo(const TestDataSetParamData& testData, ostream* os) {
	*os << "TestData(";
	PrintTo(testData.m_input, os);
	*os << ", ";
	PrintTo(testData.m_expectedData, os);
	*os << ", ";
	PrintTo(testData.m_expectedHead, os);
	*os << ")";
}

namespace {

class TestDataSet: public TestWithParam<TestDataSetParamData> {
protected:
	TestDataSet() {
	}

	virtual ~TestDataSet() {
	}
};

TEST_P(TestDataSet, SetData) {
	TestDataSetParamData testData(GetParam());

	DataSet dataSet;
	dataSet.setData(testData.m_input);

	EXPECT_EQ(testData.m_expectedData, dataSet.data());
	EXPECT_EQ(testData.m_expectedHead, dataSet.head());
}

INSTANTIATE_TEST_CASE_P(ScalesData, TestDataSet,
		Values(
				TestDataSetParamData(
						QVariantList( { -10.0, -5.0, 0.0, 5.0, 10.0 }),
						QVariantList( { 0.0, 0.25, 0.5, 0.75, 1.0 }),
						QVariant(-10.0))));

INSTANTIATE_TEST_CASE_P(TurnsBlankStringIntoNull, TestDataSet,
		Values(TestDataSetParamData(QVariantList( { "" }), QVariantList( {
				QVariant() }), QVariant())));

INSTANTIATE_TEST_CASE_P(TurnsBlankStringIntoNullWithOtherData, TestDataSet,
		Values(
				TestDataSetParamData(QVariantList( { 100.0, "", 50.0, 0.0 }),
						QVariantList( { 1.0, QVariant(), 0.5, 0.0 }),
						QVariant(100.0))));

INSTANTIATE_TEST_CASE_P(TurnsSingleValueInto0Point5, TestDataSet,
		Values(TestDataSetParamData(QVariantList( { 127.0 }), QVariantList( {
				0.5 }), QVariant(127.0))));

INSTANTIATE_TEST_CASE_P(TurnsZeroRangeInto0Point5s, TestDataSet,
		Values(
				TestDataSetParamData(QVariantList( { 27.0, 27.0 }),
						QVariantList( { 0.5, 0.5 }), QVariant(27.0))));

INSTANTIATE_TEST_CASE_P(TurnsZeroRangeInto0Point5sMoreValues, TestDataSet,
		Values(
				TestDataSetParamData(QVariantList( { 150.0, 150.0, 150.0 }),
						QVariantList( { 0.5, 0.5, 0.5 }), QVariant(150.0))));

}
// namespace
