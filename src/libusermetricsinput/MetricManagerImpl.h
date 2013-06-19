/*
 * MetricManagerImpl.h
 *
 *  Created on: 18 Jun 2013
 *      Author: pete
 */

#ifndef USERMETRICSINPUT_METRICMANAGERIMPL_H_
#define USERMETRICSINPUT_METRICMANAGERIMPL_H_

#include <libusermetricsinput/MetricManager.h>

#include <QtCore/QObject>

namespace UserMetricsInput {

class MetricManagerImpl: public MetricManager, public QObject {
public:
	explicit MetricManagerImpl(QObject *parent = 0);

	virtual ~MetricManagerImpl();

	virtual MetricPtr add(const std::string &dataSourceId,
			const std::string &formatString);
};

}

#endif // USERMETRICSINPUT_METRICMANAGERIMPL_H_
