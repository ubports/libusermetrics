#
# Regular cron jobs for the libusermetrics package
#
0 4	* * *	root	[ -x /usr/bin/libusermetrics_maintenance ] && /usr/bin/libusermetrics_maintenance
