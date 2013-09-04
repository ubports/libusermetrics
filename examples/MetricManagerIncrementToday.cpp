#include <libusermetricsinput/MetricManager.h>
#include <QtCore/QCoreApplication>

using namespace UserMetricsInput;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);

	// We start with a manager object
	MetricManagerPtr manager(MetricManager::getInstance());

	// You can hold onto this shared pointer for as long as you want
	MetricPtr metric(
			manager->add(
					MetricParameters("photo-app-photos").formatString(
							"<b>%1</b> photos taken today").emptyDataString(
							"No photos taken today").textDomain(APP_ID)));

	// The default parameters for increment cause it to be incremented by 1.0,
	// for the current user.
	metric->increment();

	return 0;
}
