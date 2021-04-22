@echo off

REM Copyright 2020-2021 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'set_project_env.bat' implementation for ECLIPSE_CDT.

REM 'set_project_env' is responsible for
REM - checking the availability of required environment variables 
REM - setting project local variables for 'build.bat' and 'run.bat' 

SET ECLIPSE_CDT_INSTALLATION_DIR=C:\ST\STM32CubeIDE_1.3.0\STM32CubeIDE
SET CUBE_PROGRAMMER_DIR=C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin
SET ECLIPSE_CDT_WORKSPACE_DIR=%~dp0\..\cdt_workspace
SET ECLIPSE_CDT_PROJECT_CONFIGURATION=Release
SET ECLIPSE_CDT_EXECUTABLE=stm32cubeidec.exe
SET ECLIPSE_CDT_PROJECT_DIR=%~dp0
SET ECLIPSE_CDT_PROJECT_NAME=application

ECHO ECLIPSE_CDT_INSTALLATION_DIR=%ECLIPSE_CDT_INSTALLATION_DIR%
ECHO CUBE_PROGRAMMER_DIR=%CUBE_PROGRAMMER_DIR%
ECHO ECLIPSE_CDT_PROJECT_DIR=%ECLIPSE_CDT_PROJECT_DIR%
ECHO ECLIPSE_CDT_PROJECT_NAME=%ECLIPSE_CDT_PROJECT_NAME%
ECHO ECLIPSE_CDT_PROJECT_CONFIGURATION=%ECLIPSE_CDT_PROJECT_CONFIGURATION%
exit /B 0
