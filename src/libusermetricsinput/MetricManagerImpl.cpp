/*
 * MetricManagerImpl.cpp
 *
 *  Created on: 18 Jun 2013
 *      Author: pete
 */

#include <libusermetricsinput/MetricImpl.h>
#include <libusermetricsinput/MetricManagerImpl.h>

using namespace std;
using namespace UserMetricsInput;

MetricManagerImpl::MetricManagerImpl(QObject *parent) :
		QObject(parent) {
}

MetricManagerImpl::~MetricManagerImpl() {
}

MetricPtr MetricManagerImpl::add(const string &dataSourceId,
		const string &formatString) {
	return MetricPtr(new MetricImpl());
}
