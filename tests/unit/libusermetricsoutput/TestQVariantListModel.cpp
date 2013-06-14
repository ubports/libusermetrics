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

#include <libusermetricsoutput/qvariantlistmodel.h>

#include <utils/QColorPrinter.h>
#include <utils/MockSignalReceiver.h>
#include <unit/libusermetricsoutput/QModelListener.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;
using namespace testutils;

namespace TestsUnitUserMetricsOutput {

class TestQVariantListModelData {
public:
	TestQVariantListModelData(const QVariantList &input, int row, int count,
			const QVariantList &aboutto, const QVariantList &res) :
			input(input), row(row), count(count), aboutto(aboutto), res(res) {
	}

	virtual ~TestQVariantListModelData() {
	}

	QVariantList input;
	int row;
	int count;
	QVariantList aboutto;
	QVariantList res;
};

class TestQVariantListModelRowsAboutToBeRemoved: public TestWithParam<
		TestQVariantListModelData> {
protected:
	TestQVariantListModelRowsAboutToBeRemoved() {
	}

	virtual ~TestQVariantListModelRowsAboutToBeRemoved() {
	}
};

INSTANTIATE_TEST_CASE_P(Data0, TestQVariantListModelRowsAboutToBeRemoved,
		Values(
				TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
						"Four", "Five" }), 1, 2,
						QVariantList( { "Two", "Three" }), QVariantList( {
								"One", "Four", "Five" }))));

INSTANTIATE_TEST_CASE_P(Data1, TestQVariantListModelRowsAboutToBeRemoved,
		Values(
				TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
						"Four", "Five" }), 0, 2,
						QVariantList( { "One", "Two" }), QVariantList( {
								"Three", "Four", "Five" }))));

INSTANTIATE_TEST_CASE_P(Data2, TestQVariantListModelRowsAboutToBeRemoved,
		Values(
				TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
						"Four", "Five" }), 3, 2,
						QVariantList( { "Four", "Five" }), QVariantList( {
								"One", "Two", "Three" }))));

INSTANTIATE_TEST_CASE_P(Data3, TestQVariantListModelRowsAboutToBeRemoved,
		Values(TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
				"Four", "Five" }), 0, 5, QVariantList( { "One", "Two", "Three",
				"Four", "Five" }), QVariantList())));

INSTANTIATE_TEST_CASE_P(Data4, TestQVariantListModelRowsAboutToBeRemoved,
		Values(
				TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
						"Four", "Five" }), 4, 2, QVariantList( { "Five", "" }),
						QVariantList( { "One", "Two", "Three", "Four" }))));

TEST_P(TestQVariantListModelRowsAboutToBeRemoved, RowsRemoved) {
	TestQVariantListModelData param(GetParam());

	QVariantList &input(param.input);
	int row(param.row);
	int count(param.count);
	QVariantList &aboutto(param.aboutto);
	QVariantList &res(param.res);

	QSharedPointer<QVariantListModel> model(new QVariantListModel(input));
	QScopedPointer<QModelListener> pListener(
			new QModelListener(aboutto, res, model));
	QObject::connect(model.data(),
			SIGNAL( rowsAboutToBeRemoved(const QModelIndex &, int, int)),
			pListener.data(),
			SLOT( rowsAboutToBeRemovedOrInserted(const QModelIndex &, int, int)));

	QObject::connect(model.data(),
			SIGNAL( rowsRemoved(const QModelIndex &, int, int)),
			pListener.data(),
			SLOT( rowsRemovedOrInserted(const QModelIndex &, int, int)));

	model->removeRows(row, count);
}

class TestQVariantListModelRowsAboutToBeInserted: public TestWithParam<
		TestQVariantListModelData> {
protected:
	TestQVariantListModelRowsAboutToBeInserted() {
	}

	virtual ~TestQVariantListModelRowsAboutToBeInserted() {
	}
};

INSTANTIATE_TEST_CASE_P(Data0, TestQVariantListModelRowsAboutToBeInserted,
		Values(
				TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
						"Four", "Five" }), 1, 2,
						QVariantList( { "Two", "Three" }),
						QVariantList( { "One", "", "", "Two", "Three", "Four",
								"Five" }))));

INSTANTIATE_TEST_CASE_P(Data1, TestQVariantListModelRowsAboutToBeInserted,
		Values(
				TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
						"Four", "Five" }), 0, 2,
						QVariantList( { "One", "Two" }), QVariantList( { "", "",
								"One", "Two", "Three", "Four", "Five" }))));

INSTANTIATE_TEST_CASE_P(Data2, TestQVariantListModelRowsAboutToBeInserted,
		Values(
				TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
						"Four", "Five" }), 3, 2,
						QVariantList( { "Four", "Five" }),
						QVariantList( { "One", "Two", "Three", "", "", "Four",
								"Five" }))));

INSTANTIATE_TEST_CASE_P(Data3, TestQVariantListModelRowsAboutToBeInserted,
		Values(TestQVariantListModelData(QVariantList( { "One", "Two", "Three",
				"Four", "Five" }), 0, 5, QVariantList( { "One", "Two", "Three",
				"Four", "Five" }), QVariantList( { "", "", "", "", "", "One",
				"Two", "Three", "Four", "Five" }))));

TEST_P(TestQVariantListModelRowsAboutToBeInserted, RowsInserted) {
	TestQVariantListModelData param(GetParam());

	QVariantList &input(param.input);
	int row(param.row);
	int count(param.count);
	QVariantList &aboutto(param.aboutto);
	QVariantList &res(param.res);

	QSharedPointer<QVariantListModel> model(new QVariantListModel(input));
	QScopedPointer<QModelListener> pListener(
			new QModelListener(aboutto, res, model));
	QObject::connect(model.data(),
			SIGNAL( rowsAboutToBeInserted(const QModelIndex &, int, int)),
			pListener.data(),
			SLOT( rowsAboutToBeRemovedOrInserted(const QModelIndex &, int, int)));

	QObject::connect(model.data(),
			SIGNAL( rowsInserted(const QModelIndex &, int, int)),
			pListener.data(),
			SLOT( rowsRemovedOrInserted(const QModelIndex &, int, int)));

	model->insertRows(row, count);
}

}

