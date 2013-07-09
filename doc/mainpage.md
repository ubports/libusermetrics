Welcome to Libusermetrics {#mainpage}
==============

Libusermetrics is a set of libraries
and a storage service designed to collect aggregated numerical data about
users. 

Getting and installing libusermetrics
-------------------------------------

### Using pre-built packages

If you just want to try out libusermetrics, or write user metrics sources,
then the easiest way is to use the pre-built packages:

 - \ref installing_prebuilt

### Building and installing from source

If you are curious about libusermetrics's internals or intend to contribute to
it, you should get the source and build it:

 - \ref building_source

Getting involved
----------------

The best place to ask questions and discuss about libusermetrics is the \#ubuntu-touch
IRC channel on freenode. 

The libusermetrics project is hosted on Launchpad: https://launchpad.net/libusermetrics

Please file bug reports at: http://bugs.launchpad.net/libusermetrics

Writing metric data sources
---------------------------

 - \ref UserMetricsInput "Libusermetrics Input API Documentation"
 
 For simple metrics which only want to increment a counter, see the following examples:
 
 - \subpage MetricManagerToday.cpp "MetricManagerToday.cpp: A simple Qt-based metric"
 - \subpage MetricManagerTodayCAPI.c "MetricManagerTodayCAPI.c: A simple C-based metric"

For more sophisticated metrics, which wish to update the history of the metric, for
example if the user has been offline for a week. Please see the following examples:

 - \subpage MetricManagerToday.cpp "MetricManagerAdvanced.cpp: A Qt-based metric"
 - \subpage MetricManagerTodayCAPI.c "MetricManagerAdvancedCAPI.c: A C-based metric"

Writing a presentation application
----------------------------------

To write a metric presentation application, there is a Qt API.

 - \ref UserMetricsOutput "Libusermetrics Output API Documentation"
 - A good example to refer to is the
   [infographics](https://bazaar.launchpad.net/~unity-team/unity8/trunk/view/head:/Greeter/Infographics.qml)
   for the Unity8 greeter.
