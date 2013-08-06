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

#include <libusermetricsoutput/ColorThemeImpl.h>

#include <testutils/QColorPrinter.h>
#include <QSignalSpy>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace UserMetricsOutput;
using namespace testing;

namespace {

class ColorThemeImplTest: public Test {
protected:
	ColorThemeImplTest() {
	}

	virtual ~ColorThemeImplTest() {
	}
};

TEST_F(ColorThemeImplTest, HasDefaultColorsWithNoArguments) {
	ColorThemeImpl colorTheme;

	EXPECT_EQ(QColor(), colorTheme.start());
	EXPECT_EQ(QColor(), colorTheme.main());
	EXPECT_EQ(QColor(), colorTheme.end());
}

TEST_F(ColorThemeImplTest, AcceptsColorsAsConstructorArguments) {
	ColorThemeImpl colorTheme(QColor(255, 0, 0), QColor(0, 255, 0),
			QColor(0, 0, 255));

	EXPECT_EQ(QColor(255, 0, 0), colorTheme.start());
	EXPECT_EQ(QColor(0, 255, 0), colorTheme.main());
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.end());
}

TEST_F(ColorThemeImplTest, MethodSetStartSendsSignal) {
	ColorThemeImpl colorTheme;

	QSignalSpy signalReceiver(&colorTheme, SIGNAL(startChanged(QColor)));

	colorTheme.setStart(QColor(255, 255, 255));
	EXPECT_EQ(QColor(255, 255, 255), colorTheme.start());

	colorTheme.setStart(QColor(255, 255, 255));
	EXPECT_EQ(QColor(255, 255, 255), colorTheme.start());

	colorTheme.setStart(QColor(255, 0, 255));
	EXPECT_EQ(QColor(255, 0, 255), colorTheme.start());

	colorTheme.setStart(QColor(255, 0, 255));
	EXPECT_EQ(QColor(255, 0, 255), colorTheme.start());

	ASSERT_EQ(2, signalReceiver.size());
	EXPECT_EQ(QVariantList() << QColor(255, 255, 255), signalReceiver.at(0));
	EXPECT_EQ(QVariantList() << QColor(255, 0, 255), signalReceiver.at(1));
}

TEST_F(ColorThemeImplTest, MethodSetMainSendsSignal) {
	ColorThemeImpl colorTheme;

	QSignalSpy signalReceiver(&colorTheme, SIGNAL(mainChanged(QColor)));

	colorTheme.setMain(QColor(0, 255, 255));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.main());

	colorTheme.setMain(QColor(0, 255, 255));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.main());

	colorTheme.setMain(QColor(0, 0, 255));
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.main());

	colorTheme.setMain(QColor(0, 0, 255));
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.main());

	ASSERT_EQ(2, signalReceiver.size());
	EXPECT_EQ(QVariantList() << QColor(0, 255, 255), signalReceiver.at(0));
	EXPECT_EQ(QVariantList() << QColor(0, 0, 255), signalReceiver.at(1));
}

TEST_F(ColorThemeImplTest, MethodSetEndSendsSignal) {
	ColorThemeImpl colorTheme;

	QSignalSpy signalReceiver(&colorTheme, SIGNAL(endChanged(QColor)));

	colorTheme.setEnd(QColor(0, 255, 255));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.end());

	colorTheme.setEnd(QColor(0, 255, 255));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.end());

	colorTheme.setEnd(QColor(0, 0, 255));
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.end());

	colorTheme.setEnd(QColor(0, 0, 255));
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.end());

	ASSERT_EQ(2, signalReceiver.size());
	EXPECT_EQ(QVariantList() << QColor(0, 255, 255), signalReceiver.at(0));
	EXPECT_EQ(QVariantList() << QColor(0, 0, 255), signalReceiver.at(1));
}

TEST_F(ColorThemeImplTest, MethodSetColorsSendsSignal) {
	ColorThemeImpl colorTheme;

	QSignalSpy startReceiver(&colorTheme, SIGNAL(startChanged(QColor)));
	QSignalSpy mainReceiver(&colorTheme, SIGNAL(mainChanged(QColor)));
	QSignalSpy endReceiver(&colorTheme, SIGNAL(endChanged(QColor)));

	colorTheme.setColors(
			ColorThemeImpl(QColor(0, 255, 255), QColor(255, 0, 255),
					QColor(255, 255, 0)));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.start());
	EXPECT_EQ(QColor(255, 0, 255), colorTheme.main());
	EXPECT_EQ(QColor(255, 255, 0), colorTheme.end());

	colorTheme.setColors(
			ColorThemeImpl(QColor(0, 255, 255), QColor(255, 0, 255),
					QColor(255, 255, 0)));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.start());
	EXPECT_EQ(QColor(255, 0, 255), colorTheme.main());
	EXPECT_EQ(QColor(255, 255, 0), colorTheme.end());

	colorTheme.setColors(
			ColorThemeImpl(QColor(1, 255, 255), QColor(255, 0, 255),
					QColor(255, 255, 0)));
	EXPECT_EQ(QColor(1, 255, 255), colorTheme.start());
	EXPECT_EQ(QColor(255, 0, 255), colorTheme.main());
	EXPECT_EQ(QColor(255, 255, 0), colorTheme.end());

	colorTheme.setColors(
			ColorThemeImpl(QColor(1, 255, 255), QColor(255, 1, 255),
					QColor(255, 255, 0)));
	EXPECT_EQ(QColor(1, 255, 255), colorTheme.start());
	EXPECT_EQ(QColor(255, 1, 255), colorTheme.main());
	EXPECT_EQ(QColor(255, 255, 0), colorTheme.end());

	colorTheme.setColors(
			ColorThemeImpl(QColor(1, 255, 255), QColor(255, 1, 255),
					QColor(255, 255, 1)));
	EXPECT_EQ(QColor(1, 255, 255), colorTheme.start());
	EXPECT_EQ(QColor(255, 1, 255), colorTheme.main());
	EXPECT_EQ(QColor(255, 255, 1), colorTheme.end());

	ASSERT_EQ(2, startReceiver.size());
	EXPECT_EQ(QVariantList() << QColor(0, 255, 255), startReceiver.at(0));
	EXPECT_EQ(QVariantList() << QColor(1, 255, 255), startReceiver.at(1));

	ASSERT_EQ(2, mainReceiver.size());
	EXPECT_EQ(QVariantList() << QColor(255, 0, 255), mainReceiver.at(0));
	EXPECT_EQ(QVariantList() << QColor(255, 1, 255), mainReceiver.at(1));

	ASSERT_EQ(2, endReceiver.size());
	EXPECT_EQ(QVariantList() << QColor(255, 255, 0), endReceiver.at(0));
	EXPECT_EQ(QVariantList() << QColor(255, 255, 1), endReceiver.at(1));
}

} // namespace
