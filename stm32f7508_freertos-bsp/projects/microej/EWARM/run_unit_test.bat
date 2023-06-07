@echo off

REM Copyright 2019-2023 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'run.bat' implementation for IAR Embedded Workbench.

REM 'run.bat' is responsible for flashing the executable file on the target device 
REM then resetting target device

REM This file has concatenated and made portable options from 'settings/lpcxpresso.Debug.driver.xcl' and 'lpcxpresso.Debug.general.xcl'
REM Any changes in the IAR settings must be reported to the command line above.

CALL "%~dp0\set_unit_test_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

IF "%~1"=="" (
	SET APPLICATION_FILE=%cd%\application.out
) ELSE (
	SET APPLICATION_FILE=%~1
)

IF NOT EXIST "%APPLICATION_FILE%" (
	echo FAILED - file '%APPLICATION_FILE%' does not exist
	exit /B 1
)


@echo on

copy /Y "%APPLICATION_FILE%" %IAREW_PROJECT_EXECUTABLE_FILE%
"%IAREW_INSTALLATION_DIR%\common\bin\cspybat" --download_only "%IAREW_INSTALLATION_DIR%\arm\bin\armproc.dll" "%IAREW_INSTALLATION_DIR%\arm\bin\armstlink2.dll" "%IAREW_PROJECT_DIR%\%IAREW_PROJECT_CONFIGURATION%\%IAREW_PROJECT_NAME%.out" --plugin="%IAREW_INSTALLATION_DIR%\arm\bin\armbat.dll" --device_macro="%IAREW_INSTALLATION_DIR%\%IAREW_PROJECT_DEVICE_MACRO%" --flash_loader="%IAREW_INSTALLATION_DIR%\%IAREW_PROJECT_FLASH_LOADER%" --backend "--endian=little" "--cpu=Cortex-M7" "--fpu=VFPv5_SP" "-p" "%IAREW_INSTALLATION_DIR%\arm\config\debugger\ST\STM32F750x8.ddf" "--semihosting" "--device=STM32750N8" "--drv_interface=SWD" "--stlink_reset_strategy=0,0" "--drv_swo_clock_setup=216000000,0,2000000" "--drv_communication_log=%~dp0\cspycomm.log" "--drv_catch_exceptions=0x000" "--drv_debug_ap=0" "--stlink_probe=stlinkv2"