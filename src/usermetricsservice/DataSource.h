/*
 * DataSource.h
 *
 *  Created on: 21 Jun 2013
 *      Author: pete
 */

#include <QDjangoModel.h>

#ifndef USERMETRICSSERVICE_DATASOURCE_H_
#define USERMETRICSSERVICE_DATASOURCE_H_

namespace UserMetricsService {

class Q_DECL_EXPORT DataSource: public QDjangoModel {
Q_OBJECT

Q_PROPERTY(QString name READ name WRITE setName)

Q_PROPERTY(QString formatString READ formatString WRITE setFormatString)

Q_CLASSINFO("name", "primary_key=true")

public:
	DataSource();

	virtual ~DataSource();

	const QString & name() const;

	void setName(const QString &name);

	const QString & formatString() const;

	void setFormatString(const QString &formatString);

protected:
	QString m_name;

	QString m_formatString;
};

}

#endif // USERMETRICSSERVICE_DATASOURCE_H_
