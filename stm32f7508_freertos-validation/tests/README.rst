
Platform Test Suites
====================

The `Platform Test Suites <https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformQualification.html#platform-test-suite>`_ validate the Abstraction Layer that implements the Low Level APIs of a Foundation Libraries by automatically running Java tests on the device.

Requirements
------------

The Platform Test Suites require a minimal configuration:

- MicroEJ SDK version ``5.1.1`` or higher.
- A MicroEJ Platform imported in the workspace, which has been built using `Platform Configuration Additions <framework/platform/README.rst>`_ version ``1.0.0`` or higher.
- A device connected to the PC both for programming and to get output traces. See Platform specific documentation for setup. 
- If the Platform requires output traces redirection (e.g. when your device dumps the standard output on a serial port), use the
  `Serial to Socket Transmitter <https://docs.microej.com/en/latest/ApplicationDeveloperGuide/serialToSocketTransmitter.html>`_ tool.

Project Configuration
---------------------

A Test Suite Project requires configurations:

- In MicroEJ SDK, import the Test Suite Project in your workspace.
- Create the local ``config.properties`` file. Copy the ``config.properties.tpl`` to ``config.properties``.
- Open the ``config.properties`` file.
- Fill the options marked as ``[required]``, particularly:

  - Target Platform: set ``target.platform.dir`` to the absolute path of the Platform being tested (the parent folder of the ``release.properties`` file and the platform sources, see `Setup a Platform for Tests <https://docs.microej.com/en/latest/ApplicationDeveloperGuide/testsuite.html#setup-a-platform-for-tests>`__),
  - `BSP Connection <https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformCreation.html#bsp-connection>`_: check the Platform specific documentation to get the kind of configured BSP connection.
  - Trace Redirection (if the Platform requires output traces redirection): set options in sync with the Serial to Socket Transmitter options.

- Check `Application Options <https://docs.microej.com/en/latest/ApplicationDeveloperGuide/applicationOptions.html>`_ declared in ``validation/microej-testsuite-common.properties``. 
  By default, options are initialized with suitable values to successfully execute the whole Test Suite and should not be changed.
  However in some particular cases, you can adjust Test Suite specific options or memory settings.
  Please refer to the section named ``Test Suite Properties`` in the ``README`` of the specific Test Suite.
  
Execution
---------
  
- Right-click on the Test Suite Project and click on ``Build Module``.
- Please refer to `Platform Qualification documentation <https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformQualification.html>`_ to know more about Test Suites and how to get reports.

Test Suite Source Code Navigation
---------------------------------

The README, CHANGELOG and source code of each Test Suite is available online:

1. Download the desired Test Suite Module: For example `com.microej.pack.fs#fs-testsuite <https://repository.microej.com/modules/com/microej/pack/fs/fs-testsuite/>`_ (refer to `Platform Qualification Test Suite Versioning <https://docs.microej.com/en/latest/PlatformDeveloperGuide/platformQualification.html#test-suite-versioning>`_ to determine the Test Suite module version).
2. Unzip the Test Suite ``RIP`` (it is a zip file): For example ``fs-testsuite-[version].rip``.
3. Open the Java archive ``JAR`` (it is a zip file): For example ``/content/javaLibs/fs-testsuite-[version].jar/``.
4. Open the desired test: For example ``/com/microej/fs/tests/constructors/TestFileInputStream.java``.

Troubleshooting
---------------

The next sections describe the typical issues you may encounter when executing a Platform Test Suite.

No activity on input stream
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following error indicates that no output have been seen by the
``Serial To Socket`` tool.

- Ensure that the COM port configured for the ``Serial To Socket`` tool is
  correct.
- Refers to the documentation of your Platform for how to connect the output traces to the Test Suite engine.

.. code-block::

   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] 1 : TraceAnalyzer ERROR :
   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] [M3] - No activity on input stream since 75 s.
   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] 
   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] 2 : TraceAnalyzer ERROR :
   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] [M5] - No encountered success/failure tag.
   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] 
   [testsuite:javaTestsuite] BUILD FAILED
   [testsuite:javaTestsuite] C:\Program Files\MicroEJ\MicroEJ-SDK-20.12\rcp\configuration\org.eclipse.osgi\11\data\repositories\microej-build-repository\com\is2t\easyant\plugins\microej-testsuite\3.4.0\microej-testsuite-harness-jpf-emb-3.4.0.xml:85: TraceAnalyzer ends with errors.

Could not open port 'COMxxx'
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following error indicates that the COM port can not be opened.

- Ensure that the COM port configured for the ``Serial To Socket`` tool is
  correct.
- Only one application may open a given COM port.  Ensure no other
  application is using this COM port.
- COM port may changed when the device is unplugged.  Ensure that the
  COM port configured for the ``Serial To Socket`` tool is correct.

.. code-block::

   serial.serialutil.SerialException: could not open port 'COM7': FileNotFoundError(2, 'The system cannot find the file specified.', None, 2)

No loaded Platform
~~~~~~~~~~~~~~~~~~

.. code-block::

   No loaded Platform.
   Possible options to load a platform are: 
   - (1) Set the property `platform-loader.target.platform.file` to a Platform file absolute path.
   - (2) Set the property `platform-loader.target.platform.dir` to a Platform directory absolute path.
   - (3) Declare a dependency in `module.ivy`.
   - (4) Copy/Paste a Platform file into the folder defined by the property `platform-loader.target.platform.dropins` (by default its value is `dropins`).
   A Platform declared using (1) or (2) is loaded prior to (3) or (4).

Ensure the property ``target.platform.dir`` is set in
``config.properties`` and points to the Platform ``source`` folder.

Could not connect to localhost:5555
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block::

   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] 1 : SocketTraceAnalyzerError ERROR :
   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] [M1] - Could not connect to localhost:5555
   [testsuite:javaTestsuite] [traceAnalyzer:socketTraceAnalyzer] 
   [testsuite:javaTestsuite] BUILD FAILED

- Ensure the ``Serial To Socket`` tool is started and configured to listen on
  the port 5555.  The host and port properties used by the Test Suite
  Engine are configured in ``config.properties``.

The Test Suite runs but some tests are failing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Ensure the Low Level Implementations tested by the Test Suite are
  correctly implemented.
- Check your Platform documentation on Test Suites. The Platform may already
  provide specific configuration (``config.properties`` and
  ``microej-testsuite-common.properties``) with some test filters (limitations or known issues).


..
   Copyright 2019-2023 MicroEJ Corp. All rights reserved.
   Use of this source code is governed by a BSD-style license that can be found with this software.
