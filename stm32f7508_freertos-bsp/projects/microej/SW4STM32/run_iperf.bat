@echo off

REM Copyright 2020-2023 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'run.bat' implementation for STM32CubeIDE.

REM 'run.bat' is responsible for flashing the executable file on the target device 
REM then resetting target device

IF "%~1"=="" (
	SET APPLICATION_FILE=%cd%\application.out
) ELSE (
	SET APPLICATION_FILE=%~f1
)

IF NOT EXIST "%APPLICATION_FILE%" (
	echo FAILED - file '%APPLICATION_FILE%' does not exist
	exit /B 1
)

CALL "%~dp0\set_iperf_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

REM Save application current directory
SET CURRENT_DIRECTORY=%CD%

REM Get arm-none-eabi-objcopy.exe path
CD "%ECLIPSE_CDT_INSTALLATION_DIR%\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.0.202111181127\tools\bin"
SET GNU_TOOLS_FOR_STM32_DIR=%CD%

REM Get STM32_Programmer_CLI.exe path
CD "%ECLIPSE_CDT_INSTALLATION_DIR%\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.0.200.202202231230\tools\bin"
SET CUBEPROGRAMMER_DIR=%CD%

REM Jump this script's directory
CD %~dp0%

@echo on 

"%GNU_TOOLS_FOR_STM32_DIR%/arm-none-eabi-objcopy.exe" -O ihex "%APPLICATION_FILE%" "%APPLICATION_FILE%.hex"
"%CUBEPROGRAMMER_DIR%\STM32_Programmer_CLI.exe" -c port=SWD mode=UR -w "%APPLICATION_FILE%.hex" -el "%CUBEPROGRAMMER_DIR%\ExternalLoader\N25Q128A_STM32F7508-DISCO.stldr" -rst

CD "%CURRENT_DIRECTORY%"
