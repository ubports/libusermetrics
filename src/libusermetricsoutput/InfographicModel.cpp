
#include <libusermetricsoutput/InfographicModelImpl.h>

using namespace UserMetricsOutput;

InfographicModel::InfographicModel(QObject *parent) :
		QObject(parent) {
}

InfographicModel::~InfographicModel() {
}

/**
 * Factory methods
 */

InfographicModel * InfographicModel::getInstance() {
	return new InfographicModelImpl();
}
