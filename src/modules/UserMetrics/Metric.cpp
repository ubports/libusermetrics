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
 */

#include <modules/UserMetrics/Metric.h>
#include <libusermetricscommon/Localisation.h>

#include <stdexcept>
#include <cmath>
#include <cfloat>
#include <QDebug>

Metric::Metric(QObject *parent) :
		QObject(parent), m_componentComplete(false), m_minimum(NAN), m_maximum(
		NAN) {
	try {
		m_metricManager.reset(UserMetricsInput::MetricManager::getInstance());
	} catch (std::exception &e) {
		qWarning() << _("Failed to connect to metrics service:") << e.what();
	}
}

Metric::~Metric() {
}

QString Metric::name() const {
	return m_name;
}

void Metric::setName(const QString &name) {
	if (name != m_name) {
		m_name = name;
		Q_EMIT nameChanged();
		registerMetric();
	}
}

QString Metric::format() const {
	return m_format;
}

void Metric::setFormat(const QString &format) {
	if (format != m_format) {
		m_format = format;
		Q_EMIT formatChanged();
		registerMetric();
	}
}

QString Metric::emptyFormat() const {
	return m_emptyFormat;
}

void Metric::setEmptyFormat(const QString &emptyFormat) {
	if (emptyFormat != m_emptyFormat) {
		m_emptyFormat = emptyFormat;
		Q_EMIT emptyFormatChanged();
		registerMetric();
	}
}

QString Metric::domain() const {
	return m_domain;
}

void Metric::setDomain(const QString &domain) {
	if (domain != m_domain) {
		m_domain = domain;
		Q_EMIT domainChanged();
		registerMetric();
	}
}

double Metric::minimum() const {
	return m_minimum;
}

void Metric::setMinimum(double minimum) {
	if (m_minimum != minimum) {
		m_minimum = minimum;
		Q_EMIT minimumChanged();
		registerMetric();
	}
}

double Metric::maximum() const {
	return m_maximum;
}

void Metric::setMaximum(double maximum) {
	if (m_maximum != maximum) {
		m_maximum = maximum;
		Q_EMIT maximumChanged();
		registerMetric();
	}
}

void Metric::registerMetric() {
	if (!m_componentComplete) {
		return;
	}

	if (m_name.isEmpty() || m_format.isEmpty()) {
		return;
	}

	if (m_metricManager.isNull()) {
		return;
	}

	try {
		UserMetricsInput::MetricParameters parameters(m_name);
		parameters.formatString(m_format);

		if (!m_emptyFormat.isEmpty()) {
			parameters.emptyDataString(m_emptyFormat);
		}
		if (!m_domain.isEmpty()) {
			parameters.textDomain(m_domain);
		}
		if (!std::isnan(m_minimum)) {
			parameters.minimum(m_minimum);
		}
		if (!std::isnan(m_maximum)) {
			parameters.maximum(m_maximum);
		}

		m_metric = m_metricManager->add(parameters);
	} catch (std::exception &e) {
		qWarning() << _("Failed to register user metric:") << m_name << "\""
				<< m_format << "\": " << e.what();
	}
}

void Metric::increment(double amount) {
	if (m_metric.isNull()) {
		return;
	}

	try {
		m_metric->increment(amount);
	} catch (std::exception &e) {
		qWarning() << _("Failed to increment metric:") << e.what();
	}
}

void Metric::update(double value) {
	if (m_metric.isNull()) {
		return;
	}

	try {
		m_metric->update(value);
	} catch (std::exception &e) {
		qWarning() << _("Failed to update metric:") << e.what();
	}
}

void Metric::classBegin() {
}

void Metric::componentComplete() {
	m_componentComplete = true;
	registerMetric();
}
