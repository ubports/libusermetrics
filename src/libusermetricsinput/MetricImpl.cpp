/*
 * MetricImpl.cpp
 *
 *  Created on: 18 Jun 2013
 *      Author: pete
 */

#include <libusermetricsinput/MetricImpl.h>
#include <libusermetricsinput/MetricUpdateImpl.h>

using namespace std;
using namespace UserMetricsInput;

MetricImpl::MetricImpl(QObject *parent) :
		QObject(parent) {
}

MetricImpl::~MetricImpl() {
}

MetricUpdatePtr MetricImpl::update(const string &username) {
	return MetricUpdatePtr(new MetricUpdateImpl(*this, username));
}
