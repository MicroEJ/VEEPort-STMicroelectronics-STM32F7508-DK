..
	Copyright 2020-2023 MicroEJ Corp. All rights reserved.
	Use of this source code is governed by a BSD-style license that can be found with this software.
..

******************************
Network Core Engine Test Suite
******************************

This folder contains a ready-to-use project for testing the `Network Core Engine <https://docs.microej.com/en/latest/PlatformDeveloperGuide/networkCoreEngine.html>`_ implementation on a device.
This Test Suite will typically create TCP sockets, UDP sockets, test client side and server side...


Specifications
--------------

- Tested Foundation Library: `NET <https://repository.microej.com/modules/ej/api/net/>`_
- Test Suite Module:  `com.microej.pack.net#net-1_1-testsuite <https://repository.microej.com/modules/com/microej/pack/net/net-1_1-testsuite/>`_

Update the NET Test Suite module version in the `module.ivy
<java-testsuite-runner-net/module.ivy>`_ to match the requirement of the platform
tested.

Please refer to `Platform Qualification Test Suite Versioning
<https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformQualification.html#test-suite-versioning>`_
to determine the Network Core Engine Test Suite module version.

Requirements
------------

- See Platform Test Suites `documentation <../README.rst>`_.

Usage
-----

- In MicroEJ SDK, import the ``java-testsuite-runner-net`` project in your workspace.
- Follow the configuration and execution steps described in Platform Test Suites `documentation <../README.rst>`_.

Test Suite Properties
---------------------

The NET Test Suite requires specific properties depending on the implementation and test environment.
All the properties described in this section are defined in the file `microej-testsuite-common.properties <java-testsuite-runner-net/validation/microej-testsuite-common.properties>`_
and prefixed by ``microej.java.property.``.

- Common properties:

  - ``testsuite.preferipv6`` (optional, false by default): set to true if the tests must use IPv6 by default.
  - To use the service for the WiFi initialization add the following dependency in your ``module.ivy``: ``<dependency org="com.microej.pack.net" name="net-1_1-testsuite-wifi" rev="2.1.0"/>``.
    - WiFi SSID and passphrase are set with respectively with the properties ``wifi.ssid`` and ``wifi.passphrase``.

- Properties to define if ``testsuite.preferipv6`` is ``false``:

  - ``wrong.machine.ipv4``: An invalid IPv4 address on the local network
  - ``remote.machine.ipv4``: IPv4 address of the testsuite server
  - ``netif.ipv4.name``: network interface name with an IPv4

- Properties to define if ``testsuite.preferipv6`` is ``true``:

  - ``wrong.machine.ipv6``: An invalid IPv6 address on the local network
  - ``remote.machine.ipv6``: IPv6 address of the testsuite server
  - ``netif.ipv6.name``: network interface name with an IPv6

- When testing a device with a dual-stack IPv6+IPv4, the following properties must be set:

  - ``remote.machine.ipv4``: see above
  - ``netif.ipv4.name``: see above
  - ``remote.machine.ipv6``: see above
  - ``netif.ipv6.name``: see above

OpenJDK Tests
-------------

This Test Suite runs network tests from the OpenJDK project. However some of these tests need the localhost network interface to run.
If you platform or implementation does not provide this interface you can skip the OpenJDK tests.
To do so, add the pattern ``**/openjdk/**/*.class`` to the property ``test.run.excludes.pattern`` in your file ``config.properties``.

Test Suite Source Code Navigation
---------------------------------

See Platform Test Suites `documentation <../README.rst>`_.

Source code of Test Suite server application is available online: 

1. Download the desired Test Suite Module: For example `com.microej.pack.net#net-1_1-testsuite <https://repository.microej.com/modules/com/microej/pack/net/net-1_1-testsuite/>`_ (refer to `Platform Qualification Test Suite Versioning <https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformQualification.html#test-suite-versioning>`_ to determine the Test Suite module version).
2. Unzip the Test Suite ``RIP`` (it is a zip file): For example ``net-1_1-testsuite-[version].rip``.
3. Open the Java archive ``JAR`` (it is a zip file): For example ``/content/resources/net-tests/net-1_1-testsuite-remote.jar/``.
4. Open the desired test: For example ``/com/microej/net/test/integration/remoteapp/TestServerSocketAccept.java``.

Troubleshooting
---------------

See Platform Test Suites `documentation <../README.rst>`_.

Debug Test Suite Server Application
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Test Suite runs a server application for which logs are only accessible via a TCP socket.
In order to display the logs, the following steps are required:

- Specify the log server address with the property ``remoteapp.logging.host`` (by default ``localhost``) in ``config.properties`` file.
- Specify the log server port with the property ``remoteapp.logging.port`` (set it to ``0`` to disable the remote log feature ; this is the default value) in ``config.properties`` file.
- Start a log server. You can use netcat: ``nc -l <port>`` (where ``<port>`` is the port specified in the previous property).
