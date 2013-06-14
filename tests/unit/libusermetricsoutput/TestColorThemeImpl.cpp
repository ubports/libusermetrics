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

#include <utils/QColorPrinter.h>
#include <utils/MockSignalReceiver.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace UserMetricsOutput;
using namespace testing;
using namespace testutils;

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

	StrictMock<MockSignalReceiverQColor> signalReceiver;
	EXPECT_CALL(signalReceiver, receivedSignal(QColor(255, 255, 255))).Times(1);
	EXPECT_CALL(signalReceiver, receivedSignal(QColor(255, 0, 255))).Times(1);

	QObject::connect(&colorTheme, SIGNAL(startChanged(QColor)), &signalReceiver,
			SLOT(receivedSignal(QColor)));

	colorTheme.setStart(QColor(255, 255, 255));
	EXPECT_EQ(QColor(255, 255, 255), colorTheme.start());

	colorTheme.setStart(QColor(255, 255, 255));
	EXPECT_EQ(QColor(255, 255, 255), colorTheme.start());

	colorTheme.setStart(QColor(255, 0, 255));
	EXPECT_EQ(QColor(255, 0, 255), colorTheme.start());

	colorTheme.setStart(QColor(255, 0, 255));
	EXPECT_EQ(QColor(255, 0, 255), colorTheme.start());
}

TEST_F(ColorThemeImplTest, MethodSetMainSendsSignal) {
	ColorThemeImpl colorTheme;

	StrictMock<MockSignalReceiverQColor> signalReceiver;
	EXPECT_CALL(signalReceiver, receivedSignal(QColor(0, 255, 255))).Times(1);
	EXPECT_CALL(signalReceiver, receivedSignal(QColor(0, 0, 255))).Times(1);

	QObject::connect(&colorTheme, SIGNAL(mainChanged(QColor)), &signalReceiver,
			SLOT(receivedSignal(QColor)));

	colorTheme.setMain(QColor(0, 255, 255));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.main());

	colorTheme.setMain(QColor(0, 255, 255));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.main());

	colorTheme.setMain(QColor(0, 0, 255));
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.main());

	colorTheme.setMain(QColor(0, 0, 255));
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.main());
}

TEST_F(ColorThemeImplTest, MethodSetEndSendsSignal) {
	ColorThemeImpl colorTheme;

	StrictMock<MockSignalReceiverQColor> signalReceiver;
	EXPECT_CALL(signalReceiver, receivedSignal(QColor(0, 255, 255))).Times(1);
	EXPECT_CALL(signalReceiver, receivedSignal(QColor(0, 0, 255))).Times(1);

	QObject::connect(&colorTheme, SIGNAL(endChanged(QColor)), &signalReceiver,
			SLOT(receivedSignal(QColor)));

	colorTheme.setEnd(QColor(0, 255, 255));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.end());

	colorTheme.setEnd(QColor(0, 255, 255));
	EXPECT_EQ(QColor(0, 255, 255), colorTheme.end());

	colorTheme.setEnd(QColor(0, 0, 255));
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.end());

	colorTheme.setEnd(QColor(0, 0, 255));
	EXPECT_EQ(QColor(0, 0, 255), colorTheme.end());
}

TEST_F(ColorThemeImplTest, MethodSetColorsSendsSignal) {
	ColorThemeImpl colorTheme;

	StrictMock<MockSignalReceiverQColor> startReceiver;
	EXPECT_CALL(startReceiver, receivedSignal(QColor(0, 255, 255))).Times(1);
	EXPECT_CALL(startReceiver, receivedSignal(QColor(1, 255, 255))).Times(1);

	StrictMock<MockSignalReceiverQColor> mainReceiver;
	EXPECT_CALL(mainReceiver, receivedSignal(QColor(255, 0, 255))).Times(1);
	EXPECT_CALL(mainReceiver, receivedSignal(QColor(255, 1, 255))).Times(1);

	StrictMock<MockSignalReceiverQColor> endReceiver;
	EXPECT_CALL(endReceiver, receivedSignal(QColor(255, 255, 0))).Times(1);
	EXPECT_CALL(endReceiver, receivedSignal(QColor(255, 255, 1))).Times(1);

	QObject::connect(&colorTheme, SIGNAL(startChanged(QColor)), &startReceiver,
			SLOT(receivedSignal(QColor)));
	QObject::connect(&colorTheme, SIGNAL(mainChanged(QColor)), &mainReceiver,
			SLOT(receivedSignal(QColor)));
	QObject::connect(&colorTheme, SIGNAL(endChanged(QColor)), &endReceiver,
			SLOT(receivedSignal(QColor)));

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
}

} // namespace
