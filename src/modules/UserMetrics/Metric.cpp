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

#include "Metric.h"

#include <QDebug>
#include <libusermetricsinput/MetricManager.h>

Metric::Metric(QObject *parent) :
    QObject(parent),
    m_metric(0)
{

}

QString Metric::name() const
{
    return m_name;
}

void Metric::setName(QString &name)
{
    if (name != m_name) {
        m_name = name;
        Q_EMIT nameChanged();
        registerMetric();
    }
}

QString Metric::format() const
{
    return m_format;
}

void Metric::setFormat(QString &format)
{
    if (format != m_format) {
        m_format = format;
        Q_EMIT formatChanged();
        registerMetric();
    }
}

QString Metric::emptyFormat() const
{
    return m_emptyFormat;
}

void Metric::setEmptyFormat(QString &emptyFormat)
{
    if (emptyFormat != m_emptyFormat) {
        m_emptyFormat = emptyFormat;
        Q_EMIT emptyFormatChanged();
        registerMetric();
    }
}

QString Metric::domain() const
{
    return m_domain;
}

void Metric::setDomain(QString &domain)
{
    if (domain != m_domain) {
        m_domain = domain;
        Q_EMIT domainChanged();
        registerMetric();
    }
}

void Metric::registerMetric()
{
    if (!m_name.isEmpty() && !m_format.isEmpty()) {
        UserMetricsInput::MetricManager* manager;
        manager = UserMetricsInput::MetricManager::getInstance();
        if (manager) {
            UserMetricsInput::MetricPtr metric = manager->add(m_name, m_format, m_emptyFormat, m_domain);
            if (metric == 0) {
                qWarning() << "Failed to register user metric:" << m_name << "\"" << m_format << "\"";
            } else if (metric != m_metric) {
                m_metric = metric;
            }
        }
    }
}

void Metric::increment(double amount)
{
    if (m_metric) {
        m_metric->increment(amount);
    }
}

void Metric::update(double value)
{
    if (m_metric) {
        m_metric->update(value);
    }
}
