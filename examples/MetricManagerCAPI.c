#include <libusermetricsinput/usermetricsinput.h>

int main(int argc, char *argv[]) {
	// We start with a manager object
	UserMetricsInputMetricManager metricManager =
			usermetricsinput_metricmanager_new();

	// You can hold onto this shared object for as long as you want
	UserMetricsInputMetric metric = usermetricsinput_metricmanager_add(
			metricManager, "twitter", "<b>%1</b> tweets received today",
			"No tweets today", "myapptextdomain");

	// The update is sent when the update object is destroyed.
	// Providing a blank username string will use the current
	// user according to the USER environment variable.
	UserMetricsInputMetricUpdate metricUpdate = usermetricsinput_metric_update(
			metric, "username");

	// The data is ordered starting from today, going backwards in time
	usermetricsinput_metricupdate_add_data(metricUpdate, 1.0);
	usermetricsinput_metricupdate_add_null(metricUpdate);
	usermetricsinput_metricupdate_add_data(metricUpdate, 0.1);
	usermetricsinput_metricupdate_delete(metricUpdate);

	usermetricsinput_metricmanager_delete(metricManager);

	return 0;
}

