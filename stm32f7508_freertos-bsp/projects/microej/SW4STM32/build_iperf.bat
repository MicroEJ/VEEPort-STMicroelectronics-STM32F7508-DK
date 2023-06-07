@echo off

REM Copyright 2020-2023 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'build.bat' implementation for Eclipse CDT.

REM 'build.bat' is responsible for producing the executable file 
REM then copying this executable file to the current directory where it has been executed to a file named 'application.out'

REM Reset ERRORLEVEL between multiple run in the same shell
SET ERRORLEVEL=0

REM Save application current directory and jump this script's directory
SET CURRENT_DIRECTORY=%CD%

CALL "%~dp0\set_iperf_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

IF DEFINED APPLY_FIRST_BSP_COMPILE_HOOKS (
	CD /D "%~dp0\..\scripts"
	CALL RunAtFirstBSPCompile.bat
	CD /D "%~dp0"
)

CD /D %~dp0%

@echo on

"%ECLIPSE_CDT_INSTALLATION_DIR%\%ECLIPSE_CDT_EXECUTABLE%" --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "%ECLIPSE_CDT_WORKSPACE_DIR%" -import "." && (
	SET ERRORLEVEL=0
	) || (
	SET ERRORLEVEL=1
)
IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

"%ECLIPSE_CDT_INSTALLATION_DIR%\%ECLIPSE_CDT_EXECUTABLE%" --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "%ECLIPSE_CDT_WORKSPACE_DIR%" -build "%ECLIPSE_CDT_PROJECT_NAME%/%ECLIPSE_CDT_PROJECT_CONFIGURATION%" && (
	SET ERRORLEVEL=0
	) || (
	SET ERRORLEVEL=1
)

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM copy the generated .elf file
COPY "%ECLIPSE_CDT_PROJECT_DIR%\%ECLIPSE_CDT_PROJECT_CONFIGURATION%\%ECLIPSE_CDT_PROJECT_NAME%.elf" "%CURRENT_DIRECTORY%\application.out" && (
	SET ERRORLEVEL=0
	) || (
	SET ERRORLEVEL=1
)

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

cd /D "%CURRENT_DIRECTORY%"
