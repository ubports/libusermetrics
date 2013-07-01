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

#include <QDjangoModel.h>

#ifndef USERMETRICSSERVICE_DATASOURCE_H_
#define USERMETRICSSERVICE_DATASOURCE_H_

namespace UserMetricsService {

class Q_DECL_EXPORT DataSource: public QDjangoModel {
Q_OBJECT

Q_PROPERTY(int id READ id WRITE setId)

Q_PROPERTY(QString name READ name WRITE setName)

Q_PROPERTY(QString formatString READ formatString WRITE setFormatString)

Q_CLASSINFO("id", "primary_key=true auto_increment=true")

Q_CLASSINFO("name", "unique=true")

public:
	explicit DataSource(QObject *parent = 0);

	virtual ~DataSource();

	static void findById(int id, DataSource *dataSource);

	static void findByName(const QString &name, DataSource *dataSource);

	static bool exists(const QString &name);

	int id() const;

	void setId(int id);

	const QString & name() const;

	void setName(const QString &name);

	const QString & formatString() const;

	void setFormatString(const QString &formatString);

protected:
	int m_id;

	QString m_name;

	QString m_formatString;
};

}

#endif // USERMETRICSSERVICE_DATASOURCE_H_
