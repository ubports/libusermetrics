#include <libusermetricsinput/usermetricsinput.h>

int main(int argc, char *argv[]) {
	// We start with a manager object
	UserMetricsInputMetricManager metricManager =
			usermetricsinput_metricmanager_new();

	// You can hold onto this shared object for as long as you want
	UserMetricsInputMetric metric = usermetricsinput_metricmanager_add(
			metricManager, "photo-app-photos", "<b>%1</b> photos taken today",
			"No photos today", APP_ID);

	// Providing a blank username string will use the current
	// user according to the USER environment variable.
	usermetricsinput_metric_update_today(metric, photo_library_get_size(), "");

	// Now we're done with the manager
	usermetricsinput_metricmanager_delete(metricManager);

	return 0;
}

