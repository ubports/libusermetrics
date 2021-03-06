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

#ifndef USERMETRICSOUTPUT_DATASET_H_
#define USERMETRICSOUTPUT_DATASET_H_

#include <libusermetricsoutput/ColorThemeImpl.h>
#include <libusermetricsoutput/DataSource.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QVariantList>
#include <QtCore/QDate>

namespace UserMetricsOutput {

class DataSet;

typedef QSharedPointer<DataSet> DataSetPtr;

class DataSet: public QObject {
Q_OBJECT

Q_PROPERTY(QDate lastUpdated READ lastUpdated NOTIFY lastUpdatedChanged FINAL)

Q_PROPERTY(const QVariantList data READ data NOTIFY dataChanged FINAL)

Q_PROPERTY(const QVariant head READ head NOTIFY headChanged FINAL)

public:
	explicit DataSet(DataSourcePtr dataSource, QObject* parent = 0);

	virtual ~DataSet();

	const QVariantList & data() const;

	const QDate & lastUpdated() const;

	const QVariant & head() const;

public Q_SLOTS:
	void update(const uint lastUpdated, const QVariantList &data);

	void setData(const QVariantList &data);

	void setLastUpdated(const QDate &lastUpdated);

	void optionsChanged(const QVariantMap &options);

Q_SIGNALS:
	void lastUpdatedChanged(const QDate &lastUpdated);

	void dataChanged(const QVariantList *data);

	void headChanged(const QVariant &head);

protected:
	void scaleData();

	DataSourcePtr m_dataSource;

	QDate m_lastUpdated;

	QVariantList m_data;

	QVariantList m_originalData;

	QVariant m_head;

};

}

#endif // USERMETRICSOUTPUT_DATASET_H_
