/*
 * MetricImpl.h
 *
 *  Created on: 18 Jun 2013
 *      Author: pete
 */

#ifndef USERMETRICSINPUT_METRICIMPL_H_
#define USERMETRICSINPUT_METRICIMPL_H_

#include <libusermetricsinput/Metric.h>

#include <QtCore/QObject>

namespace UserMetricsInput {

class MetricImpl: public Metric, public QObject {
public:
	explicit MetricImpl(QObject *parent = 0);

	virtual ~MetricImpl();

	virtual MetricUpdatePtr update(const std::string &username);
};

}

#endif // USERMETRICSINPUT_METRICIMPL_H_
