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
#include <libusermetricsinput/MetricUpdateImpl.h>

using namespace std;
using namespace UserMetricsInput;

MetricImpl::MetricImpl(const std::string &dataSourceId,
		const std::string &formatString, QObject *parent) :
		QObject(parent), m_dataSourceId(QString::fromStdString(dataSourceId)), m_formatString(
				QString::fromStdString(formatString)) {
}

MetricImpl::~MetricImpl() {
}

MetricUpdatePtr MetricImpl::update(const string &username) {
	return MetricUpdatePtr(new MetricUpdateImpl(*this, username));
}
