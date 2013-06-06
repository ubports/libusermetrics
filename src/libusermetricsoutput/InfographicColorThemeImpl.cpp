/*
 * InfographicColorThemeImpl.cpp
 *
 *  Created on: 6 Jun 2013
 *      Author: pete
 */

#include <libusermetricsoutput/InfographicColorThemeImpl.h>

using namespace UserMetricsOutput;

InfographicColorThemeImpl::InfographicColorThemeImpl(QObject *parent) :
		InfographicColorTheme(parent) {
}

InfographicColorThemeImpl::InfographicColorThemeImpl(const QColor &start,
		const QColor &main, const QColor &end, QObject *parent) :
		InfographicColorTheme(parent), m_start(start), m_main(main), m_end(end) {
}

InfographicColorThemeImpl::~InfographicColorThemeImpl() {
}

void InfographicColorThemeImpl::setColors(const InfographicColorTheme &other) {
	setStart(other.start());
	setMain(other.main());
	setEnd(other.end());
}

void InfographicColorThemeImpl::setStart(const QColor &color) {
	if (m_start != color) {
		m_start = color;
		startChanged(m_start);
	}
}

void InfographicColorThemeImpl::setMain(const QColor &color) {
	if (m_main != color) {
		m_main = color;
		mainChanged(m_main);
	}
}

void InfographicColorThemeImpl::setEnd(const QColor &color) {
	if (m_end != color) {
		m_end = color;
		endChanged(m_end);
	}
}

QColor InfographicColorThemeImpl::start() const {
	return m_start;
}

QColor InfographicColorThemeImpl::main() const {
	return m_main;
}

QColor InfographicColorThemeImpl::end() const {
	return m_end;
}
