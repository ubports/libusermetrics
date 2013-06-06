#include "InfographicData.h"

using namespace UserMetricsOutput;

int InfographicData::calculateLength() {
	int day(m_firstMonth.size());
	auto it = m_firstMonth.end(), end = m_firstMonth.begin();
	while (it != end) {
		--it;
		--day;
		if (!it->isNull()) {
			return day;
		}
	}

	return -1;
}

InfographicData::InfographicData(QObject *parent) :
		QObject(parent), m_firstColor(this), m_secondColor(this) {
	m_length = calculateLength();
}

InfographicData::InfographicData(const QString &label,
		const InfographicColorTheme &firstColor, const QVariantList &firstMonth,
		const InfographicColorTheme &secondColor,
		const QVariantList &secondMonth, QObject* parent) :
		QObject(parent), m_label(label), m_firstColor(this), m_firstMonth(
				firstMonth), m_secondColor(this), m_secondMonth(secondMonth) {
	m_length = calculateLength();
	m_firstColor.setColors(firstColor);
	m_secondColor.setColors(secondColor);
}

InfographicData::~InfographicData() {
}

const QString & InfographicData::label() const {
	return m_label;
}

const InfographicColorTheme & InfographicData::firstColor() const {
	return m_firstColor;
}

const QVariantList & InfographicData::firstMonth() const {
	return m_firstMonth;
}

const InfographicColorTheme & InfographicData::secondColor() const {
	return m_secondColor;
}

const QVariantList & InfographicData::secondMonth() const {
	return m_secondMonth;
}

int InfographicData::length() const {
	return m_length;
}
