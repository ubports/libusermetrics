#include <libusermetricsinput/usermetricsinput.h>

int main(int argc, char *argv[]) {
	// We start with a manager object.
	// This can be used as a long-lived object to reduce DBus traffic.
	UserMetricsInputMetricManager metricManager =
			usermetricsinput_metricmanager_new();

	// Create a parameters object
	UserMetricsInputMetricParameters parameters =
			usermetricsinput_metricparameters_new("twitter");

	// Set its attributes
	usermetricsinput_metricparameters_set_format_string(parameters,
			"<b>%1</b> tweets received today");
	usermetricsinput_metricparameters_set_empty_data_string(parameters,
			"No tweets today");
	usermetricsinput_metricparameters_set_text_domain(parameters,
			"myapptextdomain");

	// You can hold onto this shared object for as long as you want.
	// It will be destroyed when the manager is destroyed.
	UserMetricsInputMetric metric = usermetricsinput_metricmanager_add(
			metricManager, parameters);

	// Remember to free the parameters object after use
	usermetricsinput_metricparameters_delete(parameters);

	// The update is sent when the update object is destroyed.
	// Providing a blank username string will use the current
	// user according to the USER environment variable.
	UserMetricsInputMetricUpdate metricUpdate = usermetricsinput_metric_update(
			metric, "username");

	// The data is ordered starting from today, going backwards in time
	usermetricsinput_metricupdate_add_data(metricUpdate, 1.0);
	usermetricsinput_metricupdate_add_data(metricUpdate, 0.1);
	usermetricsinput_metricupdate_add_data(metricUpdate, -5.0);

	// Delete and dispatch the metri update
	usermetricsinput_metricupdate_delete(metricUpdate);

	// Free the manager when done with it
	usermetricsinput_metricmanager_delete(metricManager);

	return 0;
}

