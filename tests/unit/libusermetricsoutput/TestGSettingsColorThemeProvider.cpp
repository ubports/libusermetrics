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

#include <libusermetricsoutput/GSettingsColorThemeProvider.h>

#include <QtCore/QDir>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace UserMetricsOutput;
using namespace testing;

namespace {

class TestGSettingsColorThemeProvider: public Test {
protected:
	TestGSettingsColorThemeProvider() {
		qputenv("USERMETRICS_COLOR_SCHEMA_BASEDIR",
		USERMETRICS_TEST_COLOR_BASEDIR);
		qputenv("USERMETRICS_NO_COLOR_SETTINGS", "1");
	}

	virtual ~TestGSettingsColorThemeProvider() {
	}
};

TEST_F(TestGSettingsColorThemeProvider, ReadsThemes) {
	qputenv("XDG_DATA_DIRS", USERMETRICS_TEST_COLOR_BASEDIR);

	GSettingsColorThemeProvider provider;

	ColorThemePtrPair themeA(provider.getColorTheme("a"));
	EXPECT_EQ(QColor("#e54c19"), themeA.first->start());
	EXPECT_EQ(QColor("#ff9900"), themeA.first->main());
	EXPECT_EQ(QColor("#e54c19"), themeA.first->end());
	EXPECT_EQ(QColor("#ff9900"), themeA.second->start());
	EXPECT_EQ(QColor("#e54c19"), themeA.second->main());
	EXPECT_EQ(QColor("#cc0000"), themeA.second->end());

	ColorThemePtrPair themeB(provider.getColorTheme("b"));
	EXPECT_EQ(QColor("#cc19cc"), themeB.first->start());
	EXPECT_EQ(QColor("#bf21bf"), themeB.first->main());
	EXPECT_EQ(QColor("#7f334c"), themeB.first->end());
	EXPECT_EQ(QColor("#ff9900"), themeB.second->start());
	EXPECT_EQ(QColor("#e54c19"), themeB.second->main());
	EXPECT_EQ(QColor("#cc0000"), themeB.second->end());

	ColorThemePtrPair themeC(provider.getColorTheme("c"));
	EXPECT_EQ(QColor("#cc19cc"), themeC.first->start());
	EXPECT_EQ(QColor("#7f334c"), themeC.first->main());
	EXPECT_EQ(QColor("#bf21bf"), themeC.first->end());
	EXPECT_EQ(QColor("#cc0000"), themeC.second->start());
	EXPECT_EQ(QColor("#cc0000"), themeC.second->main());
	EXPECT_EQ(QColor("#cc0000"), themeC.second->end());

	ColorThemePtrPair themeD(provider.getColorTheme("d"));
	EXPECT_EQ(QColor("#cc19cc"), themeD.first->start());
	EXPECT_EQ(QColor("#cc19cc"), themeD.first->main());
	EXPECT_EQ(QColor("#cc19cc"), themeD.first->end());
	EXPECT_EQ(QColor("#cc19cc"), themeD.second->start());
	EXPECT_EQ(QColor("#7f334c"), themeD.second->main());
	EXPECT_EQ(QColor("#bf21bf"), themeD.second->end());

	// It should wrap at this point
	ColorThemePtrPair themeE(provider.getColorTheme("e"));
	EXPECT_EQ(QColor("#e54c19"), themeE.first->start());
	EXPECT_EQ(QColor("#ff9900"), themeE.first->main());
	EXPECT_EQ(QColor("#e54c19"), themeE.first->end());
	EXPECT_EQ(QColor("#ff9900"), themeE.second->start());
	EXPECT_EQ(QColor("#e54c19"), themeE.second->main());
	EXPECT_EQ(QColor("#cc0000"), themeE.second->end());
}

TEST_F(TestGSettingsColorThemeProvider, HandlesMissingXml) {
	qputenv("XDG_DATA_DIRS", "/does/not/exist");

	GSettingsColorThemeProvider provider;

	ColorThemePtrPair themeA(provider.getColorTheme("a"));
	EXPECT_EQ(QColor(), themeA.first->start());
	EXPECT_EQ(QColor(), themeA.first->main());
	EXPECT_EQ(QColor(), themeA.first->end());
	EXPECT_EQ(QColor(), themeA.second->start());
	EXPECT_EQ(QColor(), themeA.second->main());
	EXPECT_EQ(QColor(), themeA.second->end());

	EXPECT_EQ(themeA, provider.getColorTheme("b"));
}

TEST_F(TestGSettingsColorThemeProvider, HandlesInvalidXml) {
	qputenv("XDG_DATA_DIRS",
			QDir(TEST_DATADIR).filePath("broken-theme").toUtf8());

	GSettingsColorThemeProvider provider;

	ColorThemePtrPair themeA(provider.getColorTheme("a"));
	EXPECT_EQ(QColor(), themeA.first->start());
	EXPECT_EQ(QColor(), themeA.first->main());
	EXPECT_EQ(QColor(), themeA.first->end());
	EXPECT_EQ(QColor(), themeA.second->start());
	EXPECT_EQ(QColor(), themeA.second->main());
	EXPECT_EQ(QColor(), themeA.second->end());

	EXPECT_EQ(themeA, provider.getColorTheme("b"));
}

} // namespace
