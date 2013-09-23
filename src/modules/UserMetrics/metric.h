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

#ifndef METRICSMANAGER_H
#define METRICSMANAGER_H

#include <QObject>
#include <libusermetricsinput/Metric.h>

class Metric : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)

public:
    explicit Metric(QObject *parent = 0);
    QString name() const;
    void setName(QString& name);
    QString format() const;
    void setFormat(QString& format);

public Q_SLOTS:
    void increment(double amount = 1.0);
    void update(double value);

Q_SIGNALS:
    void nameChanged();
    void formatChanged();

private:
    void registerMetric();

    QString m_name;
    QString m_format;
    UserMetricsInput::MetricPtr m_metric;
};

#endif // METRICSMANAGER_H
