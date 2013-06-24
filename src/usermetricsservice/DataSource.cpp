/*
 * DataSource.cpp
 *
 *  Created on: 21 Jun 2013
 *      Author: pete
 */

#include "DataSource.h"
#include <iostream>

using namespace std;
using namespace UserMetricsService;

DataSource::DataSource() {
	printf("DataSource: %p\n", (void*)this);
}

DataSource::~DataSource() {
}

const QString & DataSource::name() const {
	return m_name;
}

void DataSource::setName(const QString &name) {
	m_name = name;
}

const QString & DataSource::formatString() const {
	return m_formatString;
}

void DataSource::setFormatString(const QString &formatString) {
	m_formatString = formatString;
}
