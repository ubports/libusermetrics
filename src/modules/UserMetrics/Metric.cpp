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
#include <QDebug>

Metric::Metric(QObject *parent) :
		QObject(parent), m_componentComplete(false) {
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
		m_metric = m_metricManager->add(
				UserMetricsInput::MetricParameters(m_name).formatString(
						m_format).emptyDataString(m_emptyFormat).textDomain(
						m_domain));
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
