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

#include <libusermetricsoutput/SyncedDataSource.h>

using namespace UserMetricsOutput;

SyncedDataSource::SyncedDataSource(
		QSharedPointer<com::canonical::usermetrics::DataSource> interface,
		QObject *parent) :
		DataSource(interface->translationPath(), parent), m_interface(interface) {

	connect(m_interface.data(), SIGNAL(formatStringChanged(const QString &)),
			this, SLOT(setFormatString(const QString &)));
	connect(m_interface.data(), SIGNAL(emptyDataStringChanged(const QString &)),
			this, SLOT(setEmptyDataString(const QString &)));
	connect(m_interface.data(), SIGNAL(textDomainChanged(const QString &)),
			this, SLOT(setTextDomain(const QString &)));
	connect(m_interface.data(), SIGNAL(metricTypeChanged(uint)), this,
			SLOT(setTypeUint(uint)));
	connect(m_interface.data(), SIGNAL(optionsChanged(const QVariantMap &)),
			this, SLOT(setOptions(const QVariantMap &)));
	setFormatString(m_interface->formatString());
	setEmptyDataString(m_interface->emptyDataString());
	setTextDomain(m_interface->textDomain());
	setTypeUint(m_interface->metricType());
	setOptions(m_interface->options());
}

SyncedDataSource::~SyncedDataSource() {
}

void SyncedDataSource::setTypeUint(uint t) {
	MetricType type(USER);
	if (t == SYSTEM) {
		type = SYSTEM;
	}
	setType(type);
}
