@echo off

REM Copyright 2020-2022 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'run.bat' implementation for STM32CubeIDE.

REM 'run.bat' is responsible for flashing the executable file on the target device 
REM then resetting target device

REM Reset ERRORLEVEL between multiple run in the same shell
SET ERRORLEVEL=0

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

REM Uncomment (delete REM) the definition below and adjust the path for arm-none-eabi-objcopy.exe
REM SET GNU_TOOLS_FOR_STM32_DIR="%ECLIPSE_CDT_INSTALLATION_DIR%\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.0.202111181127\tools\bin"
IF NOT DEFINED GNU_TOOLS_FOR_STM32_DIR (
	ECHO The GNU_TOOLS_FOR_STM32_DIR path for arm-none-eabi-objcopy.exe is not set
	exit /B 1
)
SET GNU_TOOLS_FOR_STM32_DIR=%GNU_TOOLS_FOR_STM32_DIR:"=%
ECHO GNU_TOOLS_FOR_STM32_DIR=%GNU_TOOLS_FOR_STM32_DIR%

REM Uncomment (delete REM) the definition below and adjust the path for STM32_Programmer_CLI.exe
REM SET CUBEPROGRAMMER_DIR="%ECLIPSE_CDT_INSTALLATION_DIR%\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.0.200.202202231230\tools\bin"
IF NOT DEFINED CUBEPROGRAMMER_DIR (
	ECHO The CUBEPROGRAMMER_DIR path for STM32_Programmer_CLI.exe is not set
	exit /B 1
)
SET CUBEPROGRAMMER_DIR=%CUBEPROGRAMMER_DIR:"=%
ECHO CUBEPROGRAMMER_DIR=%CUBEPROGRAMMER_DIR%

@echo on 

"%GNU_TOOLS_FOR_STM32_DIR%\arm-none-eabi-objcopy.exe" -O ihex "%APPLICATION_FILE%" "%APPLICATION_FILE%.hex"
"%CUBEPROGRAMMER_DIR%\STM32_Programmer_CLI.exe" -c port=SWD mode=UR -w "%APPLICATION_FILE%.hex" -el "%CUBEPROGRAMMER_DIR%\ExternalLoader\N25Q128A_STM32F7508-DISCO.stldr" -rst
