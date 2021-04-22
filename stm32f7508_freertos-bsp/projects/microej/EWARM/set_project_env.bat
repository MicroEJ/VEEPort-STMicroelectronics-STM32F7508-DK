@echo off

REM Copyright 2019-2021 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'set_project_env.bat' implementation for STM32CubeIDE.

REM 'set_project_env' is responsible for
REM - checking the availability of required environment variables 
REM - setting project local variables for 'build.bat' and 'run.bat' 

SET IAREW_INSTALLATION_DIR=C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.4
SET IAREW_PROJECT_CONFIGURATION=Release
SET IAREW_PROJECT_DIR=%~dp0
SET IAREW_PROJECT_NAME=application

ECHO IAREW_INSTALLATION_DIR=%IAREW_INSTALLATION_DIR%
ECHO IAREW_PROJECT_DIR=%IAREW_PROJECT_DIR%
ECHO IAREW_PROJECT_NAME=%IAREW_PROJECT_NAME%
ECHO IAREW_PROJECT_CONFIGURATION=%IAREW_PROJECT_CONFIGURATION%
exit /B 0