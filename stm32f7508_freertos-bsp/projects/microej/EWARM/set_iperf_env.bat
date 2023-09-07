@echo off

REM Copyright 2019-2023 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'set_project_env.bat' implementation for IAR Embedded Workbench.

REM 'set_project_env' is responsible for
REM - checking the availability of required environment variables 
REM - setting project local variables for 'build.bat' and 'run.bat' 

REM Required Environment Variable
REM IAR Embedded Workbench installation directory (e.g.: C:\Program Files (x86)\IAR Systems\Embedded Workbench VERSION)
SET IAREW_INSTALLATION_DIR=C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.4

REM Set the project variables required by the build script for IAR Embedded Workbench
REM IAREW project directory that contains the project file .ewp (e.g.: %~dp0\..\)
SET IAREW_PROJECT_DIR=%~dp0
REM IAREW project file name without the extension .ewp (e.g.: Project)
SET IAREW_PROJECT_NAME=application
REM IAREW project configuration (e.g.: Debug or Release)
SET IAREW_PROJECT_CONFIGURATION=Iperf

REM Device macro file relative to IAREW_INSTALLATION_DIR (e.g.: arm\config\debugger\NXP\LPC5460x.dmac)
SET IAREW_PROJECT_DEVICE_MACRO=arm\config\debugger\ST\STM32F7xx.dmac
SET IAREW_PROJECT_DEVICE_MACRO_2=arm\config\debugger\ST\STM32F7xx_DBG.dmac

REM Device flash loader file relative to IAREW_INSTALLATION_DIR (e.g.: arm\config\flashloader\NXP\FlashNXPLPC5460xM4F512K.board)
SET IAREW_PROJECT_FLASH_LOADER=arm\config\flashloader\ST\FlashSTM32F7xx_STM32F7508-DISCO.board


SET IAREW_PROJECT_EXECUTABLE_FILE="%IAREW_PROJECT_DIR%\%IAREW_PROJECT_CONFIGURATION%\%IAREW_PROJECT_NAME%.out"

ECHO IAREW_INSTALLATION_DIR=%IAREW_INSTALLATION_DIR%
ECHO IAREW_PROJECT_DIR=%IAREW_PROJECT_DIR%
ECHO IAREW_PROJECT_NAME=%IAREW_PROJECT_NAME%
ECHO IAREW_PROJECT_CONFIGURATION=%IAREW_PROJECT_CONFIGURATION%
exit /B 0
