@echo off

REM Copyright 2020 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'run.bat' implementation for STM32CubeIDE.

REM 'run.bat' is responsible for flashing the executable file on the target device 
REM then resetting target device

CALL "%~dp0\set_project_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

REM Save application current directory and jump this script's directory
SET CURRENT_DIRECTORY=%CD%
CD %~dp0%

@echo on 

"%CUBE_PROGRAMMER_DIR%\STM32_Programmer_CLI.exe" -c port=SWD mode=UR -w "%ECLIPSE_CDT_PROJECT_CONFIGURATION%\%ECLIPSE_CDT_PROJECT_NAME%.hex" -el "%CUBE_PROGRAMMER_DIR%\ExternalLoader\N25Q128A_STM32F7508-DISCO.stldr" -rst

CD "%CURRENT_DIRECTORY%"
