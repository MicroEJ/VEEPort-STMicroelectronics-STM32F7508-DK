..
    Copyright 2021-2022 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.
..

*******************
Security Test Suite
*******************

This folder contains a ready-to-use project for testing Security implementations on a device.
This Test Suite will typically test signature algorithms implementation, certificate creation, random data generation...

Specifications
--------------

- Tested Foundation Library: `Security <https://repository.microej.com/modules/ej/api/security/>`_
- Test Suite Module: `com.microej.pack.security#security-1_4-testsuite <https://repository.microej.com/modules/com/microej/pack/security/security-1_4-testsuite>`_

Update the Security Test Suite module version in the `module.ivy
<java-testsuite-runner-security/module.ivy>`_ to match the requirement of the platform
tested.

Please refer to `Platform Qualification Test Suite Versioning
<https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformQualification.html#test-suite-versioning>`_
to determine the Security Test Suite module version.

Requirements
------------

- See Platform Test Suites `documentation <../README.rst>`_.

Usage
-----

- In MicroEJ SDK, import the ``java-testsuite-runner-security`` project in your workspace.
- Follow the configuration and execution steps described in Platform Test Suites `documentation <../README.rst>`_.

Test Suite Source Code Navigation
---------------------------------

See Platform Test Suites `documentation <../README.rst>`_.

Troubleshooting
---------------

See Platform Test Suites `documentation <../README.rst>`_.
