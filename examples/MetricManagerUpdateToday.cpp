#include <libusermetricsinput/MetricManager.h>
#include <QtCore/QCoreApplication>

using namespace UserMetricsInput;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);

	// We start with a manager object
	MetricManagerPtr manager(MetricManager::getInstance());

	// You can hold onto this shared pointer for as long as you want
	MetricPtr metric(
			manager->add("photo-app-photos", "<b>%1</b> photos taken today",
					"No photos taken today", APP_ID));

	// Get the data from somewhere
	PhotoLibrary photoLibrary;

	// The default parameters for increment cause it to update
	// the current user.
	metric->update(photoLibrary.size());

	return 0;
}
