Building the source
===================

Getting libusermetrics
----------------------

Libusermetrics is a project on Launchpad (https://launchpad.net/libusermetrics).
To grab a copy use the command:

    $ bzr branch lp:libusermetrics


Getting dependencies
--------------------

To succesfully build libusermetrics there are a few packages required. The
easiest way to get them is to use the packaging build dependencies:

    $ sudo apt-get install devscripts equivs cmake
    $ sudo mk-build-deps --install --tool "apt-get -y" --build-dep debian/control


Building libusermetrics
-----------------------

Libusermetrics is built using cmake. To make life simpler, you can use the
included wrapper script:

    $ ./build

This will run cmake in the directory

    ../${BRANCH_NAME}-build

You can also ask it to create an Eclipse project as follows:

    $ ./build -e

The next step is to build the source and run the tests:

    $ cd ../libusermetrics-build/make (-j8)
    $ make check

Installing libusermetrics
-------------------------

To install libusermetrics just use the normal make install command:

    $ make install

This will install the libusermetrics input / output libraries, storage service,
and header files to the configured installation location (/usr/local by
default). If you install to a non-standard location, keep in mind that you will
probably need to properly set the PKG_CONFIG_PATH environment variable to allow
other applications to build against libusermetrics, and LD_LIBRARY_PATH to allow
applications to find the libusermetrics libraries at runtime.
