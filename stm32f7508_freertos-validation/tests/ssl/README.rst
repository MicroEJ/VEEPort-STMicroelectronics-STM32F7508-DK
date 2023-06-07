..
    Copyright 2022-2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.
..

**************
SSL Test Suite
**************

This folder contains a ready-to-use project for testing `Secure Socket Layer <https://docs.microej.com/en/latest/PlatformDeveloperGuide/ssl.html>`_ implementations on a device.
This Test Suite will typically test secure sockets using SSL/TLS protocol.

Specifications
--------------

- Tested Foundation Library: `SSL <https://repository.microej.com/modules/ej/api/ssl/>`_
- Test Suite Module: `com.microej.pack.ssl#ssl-2_2-testsuite <https://repository.microej.com/modules/com/microej/pack/ssl/ssl-2_2-testsuite/>`_

Update the SSL Test Suite module version in the `module.ivy
<java-testsuite-runner-ssl/module.ivy>`_ to match the requirement of the platform
tested.

Please refer to `Platform Qualification Test Suite Versioning
<https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformQualification.html#test-suite-versioning>`_
to determine the SSL Test Suite module version.

Requirements
------------

- See Platform Test Suites `documentation <../README.rst>`_.

Usage
-----

- In MicroEJ SDK, import the ``java-testsuite-runner-ssl`` project in your workspace.
- Follow the configuration and execution steps described in Platform Test Suites `documentation <../README.rst>`_.

Test Suite Properties
---------------------

The NET Test Suite requires specific properties depending on the implementation and test environment.
All the properties described in this section are defined in the file `microej-testsuite-common.properties <java-testsuite-runner-net/validation/microej-testsuite-common.properties>`_
and prefixed by ``microej.java.property.``.

- WiFi SSID and passphrase are set with respectively with the properties ``wifi.ssid`` and ``wifi.passphrase``.

- Application options specific to SSL:

   - ``remote.machine.ip``: Set this property with the value of the IP address of the machine that runs the tests.
   - ``remoteapp.timeout``: Amount of time (in milliseconds) that the remote test will wait before interrupting its execution.
   - ``microej.ssl.testsuite.ssl.stack``: Property to set to ``mbedtls`` when running tests on platforms with a mbedTLS backend.

Test Suite Source Code Navigation
---------------------------------

See Platform Test Suites `documentation <../README.rst>`_.

Source code of Test Suite server application is available online: 

1. Download the desired Test Suite Module: For example `com.microej.pack.net#net-ssl-2_2-testsuite <https://repository.microej.com/modules/com/microej/pack/net/net-ssl-2_2-testsuite/>`_ (refer to `Platform Qualification Test Suite Versioning <https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformQualification.html#test-suite-versioning>`_ to determine the Test Suite module version).
2. Unzip the Test Suite ``RIP`` (it is a zip file): For example ``net-ssl-2_2-testsuite-[version].rip``.
3. Open the Java archive ``JAR`` (it is a zip file): For example ``/content/resources/ssl-tests/net-ssl-2_2-testsuite-remote.jar/``.
4. Open the desired test: For example ``/com/microej/ssl/test/remote/TestClientAuthentication_1.java``.

Troubleshooting
---------------

See Platform Test Suites `documentation <../README.rst>`_.
