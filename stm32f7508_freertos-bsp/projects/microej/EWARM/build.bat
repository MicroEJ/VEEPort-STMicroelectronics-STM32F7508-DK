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

REM Apply the git patch
CMD /C %~dp0\..\scripts\patchSTM32CubeF7.bat

REM exit early if there are errors in applying the patch
IF %ERRORLEVEL% NEQ 0 (
	ECHO "Failed to patch STM32CubeF7 SDK"
	EXIT /B %ERRORLEVEL%
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
copy /Y %IAREW_PROJECT_EXECUTABLE_FILE% "%CURRENT_DIRECTORY%\application.out" && (
	SET ERRORLEVEL=0
	) || (
	SET ERRORLEVEL=1
)

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM Restore current directory
cd /D "%CURRENT_DIRECTORY%"
