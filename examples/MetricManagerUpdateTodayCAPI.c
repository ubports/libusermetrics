#include <libusermetricsinput/usermetricsinput.h>

int main(int argc, char *argv[]) {
	// We start with a manager object
	UserMetricsInputMetricManager metricManager =
			usermetricsinput_metricmanager_new();

	// Create a parameters object
	UserMetricsInputMetricParameters parameters =
			usermetricsinput_metricparameters_new("photo-app-photos");

	// Set its attributes
	usermetricsinput_metricparameters_set_format_string(parameters,
			"<b>%1</b> photos taken today");
	usermetricsinput_metricparameters_set_empty_data_string(parameters,
			"No photos today");
	usermetricsinput_metricparameters_set_text_domain(parameters, APP_ID);

	// You can hold onto this shared object for as long as you want
	UserMetricsInputMetric metric = usermetricsinput_metricmanager_add(
			metricManager, parameters);

	// Remember to free the parameters object after use
	usermetricsinput_metricparameters_delete(parameters);

	// Providing a blank username string will use the current
	// user according to the USER environment variable.
	usermetricsinput_metric_update_today(metric, photo_library_get_size(), "");

	// Now we're done with the manager
	usermetricsinput_metricmanager_delete(metricManager);

	return 0;
}

