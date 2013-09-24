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

#ifndef MODULES_USERMETRICS_METRICSMANAGER_H
#define MODULES_USERMETRICS_METRICSMANAGER_H

#include <QObject>
#include <QQmlParserStatus>
#include <libusermetricsinput/MetricManager.h>

class Metric: public QObject, public QQmlParserStatus {
Q_OBJECT

Q_INTERFACES (QQmlParserStatus)Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)
Q_PROPERTY(QString emptyFormat READ emptyFormat WRITE setEmptyFormat NOTIFY emptyFormatChanged)
Q_PROPERTY(QString domain READ domain WRITE setDomain NOTIFY domainChanged)

public:
	explicit Metric(QObject *parent = 0);

	virtual ~Metric();

	QString name() const;

	void setName(const QString &name);

	QString format() const;

	void setFormat(const QString &format);

	QString emptyFormat() const;

	void setEmptyFormat(const QString &emptyFormat);

	QString domain() const;

	void setDomain(const QString &domain);

	void classBegin();

	void componentComplete();

public Q_SLOTS:
	void increment(double amount = 1.0);

	void update(double value);

Q_SIGNALS:
	void nameChanged();

	void formatChanged();

	void emptyFormatChanged();

	void domainChanged();

protected:
	void registerMetric();

	UserMetricsInput::MetricManagerPtr m_metricManager;

	UserMetricsInput::MetricPtr m_metric;

	bool m_componentComplete;

	QString m_name;

	QString m_format;

	QString m_emptyFormat;

	QString m_domain;
};

#endif // MODULES_USERMETRICS_METRICSMANAGER_H
