@echo off

REM Copyright 2019-2020 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'build.bat' implementation for IAR Embedded Workbench.

REM 'build.bat' is responsible for producing the executable file 
REM then copying this executable file to the current directory where it has been executed to a file named 'application.out'

CALL "%~dp0\set_project_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

@echo on

"%IAREW_INSTALLATION_DIR%\common\bin\IarBuild.exe" "%IAREW_PROJECT_DIR%\%IAREW_PROJECT_NAME%.ewp" -make %IAREW_PROJECT_CONFIGURATION% -log all
copy /Y "%IAREW_PROJECT_DIR%\%IAREW_PROJECT_CONFIGURATION%\%IAREW_PROJECT_NAME%.out" application.out