@echo off

REM Copyright 2019-2023 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'build.bat' implementation for IAR Embedded Workbench.

REM 'build.bat' is responsible for producing the executable file 
REM then copying this executable file to the current directory where it has been executed to a file named 'application.out'

REM Reset ERRORLEVEL between multiple run in the same shell
SET ERRORLEVEL=0

REM Save application current directory
SET CURRENT_DIRECTORY=%CD%

CALL "%~dp0\set_project_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

IF DEFINED APPLY_FIRST_BSP_COMPILE_HOOKS (
	CD /D "%~dp0\..\scripts"
	CALL RunAtFirstBSPCompile.bat
	CD /D "%~dp0"
)

@echo on

"%IAREW_INSTALLATION_DIR%\common\bin\IarBuild.exe" "%IAREW_PROJECT_DIR%\%IAREW_PROJECT_NAME%.ewp" -make %IAREW_PROJECT_CONFIGURATION% -log all && (
	SET ERRORLEVEL=0
	) || (
	SET ERRORLEVEL=1
)

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM copy the generated .elf file
copy /Y %IAREW_PROJECT_EXECUTABLE_FILE% application.out && (
	SET ERRORLEVEL=0
	) || (
	SET ERRORLEVEL=1
)

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM Restore current directory
cd /D "%CURRENT_DIRECTORY%"
