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

using namespace UserMetricsOutput;

ColorThemeImpl::ColorThemeImpl(QObject *parent) :
		ColorTheme(parent) {
}

ColorThemeImpl::ColorThemeImpl(const QColor &start, const QColor &main,
		const QColor &end, QObject *parent) :
		ColorTheme(parent), m_start(start), m_main(main), m_end(end) {
}

ColorThemeImpl::~ColorThemeImpl() {
}

void ColorThemeImpl::setColors(const ColorTheme &other) {
	setStart(other.start());
	setMain(other.main());
	setEnd(other.end());
}

void ColorThemeImpl::setStart(const QColor &color) {
	if (m_start != color) {
		m_start = color;
		startChanged(m_start);
	}
}

void ColorThemeImpl::setMain(const QColor &color) {
	if (m_main != color) {
		m_main = color;
		mainChanged(m_main);
	}
}

void ColorThemeImpl::setEnd(const QColor &color) {
	if (m_end != color) {
		m_end = color;
		endChanged(m_end);
	}
}

QColor ColorThemeImpl::start() const {
	return m_start;
}

QColor ColorThemeImpl::main() const {
	return m_main;
}

QColor ColorThemeImpl::end() const {
	return m_end;
}
