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

#ifndef USERMETRICS_TESTUTILS_MOCKSIGNALRECEIVER_H_
#define USERMETRICS_TESTUTILS_MOCKSIGNALRECEIVER_H_

#include <QtGui/QColor>
#include <QtCore/QObject>
#include <gmock/gmock.h>

namespace UserMetricsTestUtils {

class AbstractMockSignalReceiverInt: public QObject {
Q_OBJECT
public Q_SLOTS:
	virtual void receivedSignal(int) = 0;
};

class MockSignalReceiverInt: public AbstractMockSignalReceiverInt {
Q_OBJECT
public:
	MOCK_METHOD1(receivedSignal, void(int));
};

/***************************************/

class AbstractMockSignalReceiverQColor: public QObject {
Q_OBJECT
public Q_SLOTS:
	virtual void receivedSignal(QColor) = 0;
};

class MockSignalReceiverQColor: public AbstractMockSignalReceiverQColor {
Q_OBJECT
public:
	MOCK_METHOD1(receivedSignal, void(QColor));
};

}

#endif /* USERMETRICS_TESTUTILS_QSTRINGPRINTER_H_ */

