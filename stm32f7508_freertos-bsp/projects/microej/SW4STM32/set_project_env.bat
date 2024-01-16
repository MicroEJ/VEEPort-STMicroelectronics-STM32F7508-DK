@echo off

REM Copyright 2020-2024 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'set_project_env.bat' implementation for ECLIPSE_CDT.

REM 'set_project_env' is responsible for
REM - checking the availability of required environment variables 
REM - setting project local variables for 'build.bat' and 'run.bat' 

REM Set the project variables required by the external build script for Eclipse CDT.
SET ECLIPSE_CDT_DEFAULT_INSTALLATION_DIR=C:\ST\STM32CubeIDE_1.9.0\STM32CubeIDE
REM The Cube Programmer utility directory absolute path
SET CUBE_PROGRAMMER_DEFAULT_DIR="%ECLIPSE_CDT_DEFAULT_INSTALLATION_DIR%\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.0.200.202202231230\tools\bin"

IF NOT EXIST "%ECLIPSE_CDT_DEFAULT_INSTALLATION_DIR%" (
    IF NOT DEFINED ECLIPSE_CDT_INSTALLATION_DIR (
        ECHO ERROR: please set the environment variable ECLIPSE_CDT_INSTALLATION_DIR, refer to the VEE Port documentation for more information.
        EXIT /B 1
    )
    IF NOT DEFINED CUBE_PROGRAMMER_DIR (
        ECHO ERROR: please set the environment variable CUBE_PROGRAMMER_DIR, refer to the VEE Port documentation for more information.
        EXIT /B 1        
    )
) ELSE (
    CALL :setDefaultSTM32CubePath
)

SET ECLIPSE_CDT_EXECUTABLE=stm32cubeidec.exe
SET ECLIPSE_CDT_PROJECT_DIR=%~dp0
SET ECLIPSE_CDT_PROJECT_NAME=application
SET ECLIPSE_CDT_PROJECT_CONFIGURATION=Release
SET ECLIPSE_CDT_WORKSPACE_DIR=%~dp0\..\cdt_workspace

ECHO ECLIPSE_CDT_INSTALLATION_DIR=%ECLIPSE_CDT_INSTALLATION_DIR%
ECHO ECLIPSE_CDT_PROJECT_DIR=%ECLIPSE_CDT_PROJECT_DIR%
ECHO ECLIPSE_CDT_PROJECT_NAME=%ECLIPSE_CDT_PROJECT_NAME%
ECHO ECLIPSE_CDT_PROJECT_CONFIGURATION=%ECLIPSE_CDT_PROJECT_CONFIGURATION%
ECHO CUBE_PROGRAMMER_DIR=%CUBE_PROGRAMMER_DIR%
exit /B 0

:setDefaultSTM32CubePath
    SET ECLIPSE_CDT_INSTALLATION_DIR=%ECLIPSE_CDT_DEFAULT_INSTALLATION_DIR%
    SET CUBE_PROGRAMMER_DIR=%CUBE_PROGRAMMER_DEFAULT_DIR%
    goto:eof