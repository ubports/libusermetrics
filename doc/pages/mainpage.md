Welcome to Libusermetrics {#mainpage}
=========================

Libusermetrics enables apps to locally store interesting numerical data
for later presentation.  For example in the Ubuntu Greeter "flower"
infographic. 

 - All data is stored locally in /var/usermetrics/.
 - No data is centrally collected via a web-serivice or otherwise, and
   no data is sent over the internet.

The only data that can be stored is numerical, for example "number of
e-mails" or "number of pictures taken". No personally identifying
information is stored using this library.

**WARNING:** This API should be considered private, and is only for use by
first party applications at present. It may change or be removed at any
time.

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
 
 - \subpage MetricManagerIncrementToday.cpp "MetricManagerIncrementToday.cpp: A simple Qt-based metric"
 - \subpage MetricManagerIncrementTodayCAPI.c "MetricManagerIncrementTodayCAPI.c: A simple C-based metric"
 
For simple metrics which only want to update today's value, but cannot simply increment it,
see the following examples:

 - \subpage MetricManagerUpdateToday.cpp "MetricManagerUpdateToday.cpp: A simple Qt-based metric"
 - \subpage MetricManagerUpdateTodayCAPI.c "MetricManagerUpdateTodayCAPI.c: A simple C-based metric"

For more sophisticated metrics, which wish to update the history of the metric, for
example if the user has been offline for a week. Please see the following examples:

 - \subpage MetricManagerAdvanced.cpp "MetricManagerAdvanced.cpp: A Qt-based metric"
 - \subpage MetricManagerAdvancedCAPI.c "MetricManagerAdvancedCAPI.c: A C-based metric"

Writing a presentation application
----------------------------------

To write a metric presentation application, there is a Qt API.

 - \ref UserMetricsOutput "Libusermetrics Output API Documentation"
 - A good example to refer to is the
   [infographics](https://bazaar.launchpad.net/~unity-team/unity8/trunk/view/head:/Greeter/Infographics.qml)
   for the Unity8 greeter.
