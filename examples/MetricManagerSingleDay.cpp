
#include <libusermetricsinput/MetricManager.h>
#include <QtCore/QCoreApplication>

using namespace UserMetricsInput;

int main(int argc, char *argv[]) {
	QCoreApplication application(argc, argv);

	// We start with a manager object
	MetricManagerPtr manager(MetricManager::getInstance());

	// You can hold onto this shared pointer for as long as you want
	MetricPtr metric(
			manager->add("twitter", "<b>%1</b> tweets received today", "myapptextdomain"));

	// The update is sent when the update object is destroyed
	MetricUpdatePtr update(metric->update());

	// Get our data from somewhere
	TwitterService twitterService;

	// In this example we just set today's data
	update->addData(twitterService.getTweetCount());

	return 0;
}
