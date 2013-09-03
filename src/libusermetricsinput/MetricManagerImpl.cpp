/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <libusermetricsinput/MetricImpl.h>
#include <libusermetricsinput/MetricManagerImpl.h>
#include <libusermetricscommon/DBusPaths.h>

#include <QtDBus/QtDBus>
#include <QtCore/QDebug>

#include <stdexcept>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsInput;

class UserMetricsInput::MetricParametersPrivate {
	friend class MetricManagerImpl;
	friend class MetricParameters;

public:
	explicit MetricParametersPrivate(const QString &dataSourceId) :
			m_dataSourceId(dataSourceId), m_type(MetricType::USER) {
	}

	~MetricParametersPrivate() {
	}

protected:
	QString m_dataSourceId;

	QString m_formatString;

	QString m_emptyDataString;

	QString m_textDomain;

	MetricType m_type;

	QVariantMap m_options;
};

MetricParameters::MetricParameters(const QString &dataSourceId) :
		p(new MetricParametersPrivate(dataSourceId)) {
}

MetricParameters::~MetricParameters() {
}

MetricParameters & MetricParameters::formatString(const QString &formatString) {
	p->m_formatString = formatString;
	return *this;
}

MetricParameters & MetricParameters::emptyDataString(
		const QString &emptyDataString) {
	p->m_emptyDataString = emptyDataString;
	return *this;
}

MetricParameters & MetricParameters::textDomain(const QString &textDomain) {
	p->m_textDomain = textDomain;
	return *this;
}

MetricParameters & MetricParameters::minimum(double minimum) {
	p->m_options["minimum"] = minimum;
	return *this;
}

MetricParameters & MetricParameters::maximum(double maximum) {
	p->m_options["maximum"] = maximum;
	return *this;
}

MetricParameters & MetricParameters::type(MetricType type) {
	p->m_type = type;
	return *this;
}

MetricManagerImpl::MetricManagerImpl(const QDBusConnection &dbusConnection,
		QObject *parent) :
		MetricManager(parent), m_dbusConnection(dbusConnection), m_interface(
				DBusPaths::serviceName(), DBusPaths::userMetrics(),
				dbusConnection) {
}

MetricManagerImpl::~MetricManagerImpl() {
}

MetricPtr MetricManagerImpl::add(const QString &dataSourceId,
		const QString &formatString, const QString &emptyDataString,
		const QString &textDomain) {
	return add(
			MetricParameters(dataSourceId).formatString(formatString).emptyDataString(
					emptyDataString).textDomain(textDomain));
}

MetricPtr MetricManagerImpl::add(const MetricParameters &parameters) {
	QDBusPendingReply<QDBusObjectPath> reply(
			m_interface.createDataSource(parameters.p->m_dataSourceId,
					parameters.p->m_formatString,
					parameters.p->m_emptyDataString, parameters.p->m_textDomain,
					parameters.p->m_type, parameters.p->m_options));

	reply.waitForFinished();

	if (reply.isError()) {
		throw logic_error(reply.error().message().toStdString());
	}

	QDBusObjectPath path(reply.value());

	auto metric(m_metrics.find(parameters.p->m_dataSourceId));
	if (metric == m_metrics.end()) {
		MetricPtr newMetric(
				new MetricImpl(parameters.p->m_dataSourceId,
						parameters.p->m_formatString, path.path(),
						m_dbusConnection));
		metric = m_metrics.insert(parameters.p->m_dataSourceId, newMetric);
	}
	return *metric;
}
