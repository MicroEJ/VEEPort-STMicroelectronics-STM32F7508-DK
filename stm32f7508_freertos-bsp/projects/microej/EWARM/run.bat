@echo off

REM Copyright 2019-2022 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'run.bat' implementation for IAR Embedded Workbench.

REM 'run.bat' is responsible for flashing the executable file on the target device 
REM then resetting target device

REM This file has concatenated and made portable options from 'settings/lpcxpresso.Debug.driver.xcl' and 'lpcxpresso.Debug.general.xcl'
REM Any changes in the IAR settings must be reported to the command line above.

IF "%~1"=="" (
	SET APPLICATION_FILE=%cd%\application.out
) ELSE (
	SET APPLICATION_FILE=%~f1
)

IF NOT EXIST "%APPLICATION_FILE%" (
	echo FAILED - file '%APPLICATION_FILE%' does not exist
	exit /B 1
)

CALL "%~dp0\set_project_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

@echo on 

"%IAREW_INSTALLATION_DIR%\common\bin\cspybat" --download_only "%IAREW_INSTALLATION_DIR%\arm\bin\armproc.dll" "%IAREW_INSTALLATION_DIR%\arm\bin\armstlink2.dll" "%APPLICATION_FILE%" --plugin="%IAREW_INSTALLATION_DIR%\arm\bin\armbat.dll" --flash_loader="%IAREW_INSTALLATION_DIR%\arm\config\flashloader\ST\FlashSTM32F7xx_STM32F7508-DISCO.board" --backend "--endian=little" "--cpu=Cortex-M7" "--fpu=VFPv5_SP" "-p" "%IAREW_INSTALLATION_DIR%\arm\config\debugger\ST\STM32F750x8.ddf" "--semihosting" "--device=STM32750N8" "--drv_interface=SWD" "--stlink_reset_strategy=0,0" "--drv_swo_clock_setup=216000000,0,2000000" "--drv_communication_log=%~dp0\cspycomm.log" "--drv_catch_exceptions=0x000" "--drv_debug_ap=0" "--stlink_probe=stlinkv2" 