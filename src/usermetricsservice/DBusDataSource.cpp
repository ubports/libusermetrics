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
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <stdexcept>

#include <usermetricsservice/database/DataSource.h>
#include <usermetricsservice/DBusDataSource.h>
#include <usermetricsservice/DataSourceAdaptor.h>
#include <usermetricsservice/TranslationLocator.h>
#include <libusermetricscommon/DBusPaths.h>
#include <libusermetricscommon/Localisation.h>

#include <QDjangoQuerySet.h>

using namespace std;
using namespace UserMetricsCommon;
using namespace UserMetricsService;

DBusDataSource::DBusDataSource(int id, const QString &name,
		const QString &packageId, QDBusConnection &dbusConnection,
		QSharedPointer<TranslationLocator> translationLocator, QObject *parent) :
		QObject(parent), m_dbusConnection(dbusConnection), m_adaptor(
				new DataSourceAdaptor(this)), m_id(id), m_path(
				DBusPaths::dataSource(m_id)), m_name(name), m_packageId(
				packageId), m_translationLocator(translationLocator) {

	// DBus setup
	m_dbusConnection.registerObject(m_path, this);
}

DBusDataSource::~DBusDataSource() {
	QDBusConnection connection(QDBusConnection::sessionBus());
	connection.unregisterObject(m_path);
}

QString DBusDataSource::path() const {
	return m_path;
}

QString DBusDataSource::translationPath() const {
	return m_translationLocator->locate(m_packageId);
}

QString DBusDataSource::name() const {
	return m_name;
}

QString DBusDataSource::formatString() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.formatString();
}

void DBusDataSource::setFormatString(const QString &formatString) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	if (formatString != dataSource.formatString()) {
		dataSource.setFormatString(formatString);
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
		m_adaptor->formatStringChanged(formatString);
	}
}

QString DBusDataSource::emptyDataString() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.emptyDataString();
}

void DBusDataSource::setEmptyDataString(const QString &emptyDataString) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	if (emptyDataString != dataSource.emptyDataString()) {
		dataSource.setEmptyDataString(emptyDataString);
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
		m_adaptor->emptyDataStringChanged(emptyDataString);
	}
}

QString DBusDataSource::textDomain() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.textDomain();
}

void DBusDataSource::setTextDomain(const QString &textDomain) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	if (textDomain != dataSource.textDomain()) {
		dataSource.setTextDomain(textDomain);
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
		m_adaptor->textDomainChanged(textDomain);
	}
}

void DBusDataSource::setSecret(const QString &secret) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	if (secret != dataSource.secret()) {
		dataSource.setSecret(secret);
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
	}
}

unsigned int DBusDataSource::metricType() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.type();
}

void DBusDataSource::setMetricType(unsigned int type) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	unsigned int t(dataSource.type());
	if (type != t) {
		dataSource.setType(type);
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
		m_adaptor->metricTypeChanged(type);
	}
}

QVariantMap DBusDataSource::generateOptions(
		const DataSource &dataSource) const {
	QVariantMap options;
	if (dataSource.hasMinimum()) {
		options["minimum"] = dataSource.minimum();
	}
	if (dataSource.hasMaximum()) {
		options["maximum"] = dataSource.maximum();
	}
	return options;
}

bool DBusDataSource::hasMinimum() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.hasMinimum();
}

void DBusDataSource::setMinimum(double minimum) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	bool changed(false);
	if (!dataSource.hasMinimum()) {
		dataSource.setHasMinimum(true);
		changed = true;
	}
	if (dataSource.minimum() != minimum) {
		dataSource.setMinimum(minimum);
		changed = true;
	}
	if (changed) {
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
		m_adaptor->optionsChanged(generateOptions(dataSource));
	}
}

double DBusDataSource::minimum() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.minimum();
}

void DBusDataSource::noMinimum() {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	if (dataSource.hasMinimum()) {
		dataSource.setHasMinimum(false);
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
		m_adaptor->optionsChanged(generateOptions(dataSource));
	}
}

bool DBusDataSource::hasMaximum() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.hasMaximum();
}

void DBusDataSource::setMaximum(double maximum) {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	bool changed(false);
	if (!dataSource.hasMaximum()) {
		dataSource.setHasMaximum(true);
		changed = true;
	}
	if (dataSource.maximum() != maximum) {
		dataSource.setMaximum(maximum);
		changed = true;
	}
	if (changed) {
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
		m_adaptor->optionsChanged(generateOptions(dataSource));
	}
}

double DBusDataSource::maximum() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return dataSource.maximum();
}

void DBusDataSource::noMaximum() {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	if (dataSource.hasMaximum()) {
		dataSource.setHasMaximum(false);
		if (!dataSource.save()) {
			throw logic_error(_("Could not save data source"));
		}
		m_adaptor->optionsChanged(generateOptions(dataSource));
	}
}

QVariantMap DBusDataSource::options() const {
	DataSource dataSource;
	DataSource::findById(m_id, &dataSource);
	return generateOptions(dataSource);
}
