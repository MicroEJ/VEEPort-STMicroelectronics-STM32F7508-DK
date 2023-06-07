@echo off
SETLOCAL ENABLEEXTENSIONS

REM BAT
REM Copyright 2020-2023 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM Init. the echosystem afther the repository cloning of the working branch.
REM This will apply patch (CubeF7-MicroEJ.patch)

REM Reset ERRORLEVEL between multiple run in the same shell
SET ERRORLEVEL=0

REM Get current directory
SET CURRENT_DIR=%CD%
ECHO CURRENT_DIR=%CURRENT_DIR%
CD "%~dp0"

REM Copy the patch file to the submodule directory (STM32CubeF7)
COPY /Y /Z /B "..\..\..\sdk\CubeF7_patch\CubeF7-MicroEJ.patch" "..\..\..\sdk\STM32CubeF7\CubeF7-MicroEJ.patch"
IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM STM32CubeF7 path to aplly patch
CD %CURRENT_DIR%\..\..\..\sdk\STM32CubeF7

REM Needed only for Jenkins to remove new files added by patch when start a new pipline
IF EXIST Middlewares\Third_Party\mbedTLS\microej_readme.txt (
    DEL  /F /Q Middlewares\Third_Party\mbedTLS\microej_readme.txt
    DEL  /F /Q Middlewares\Third_Party\FreeRTOS\Source\microej_readme.txt
    DEL  /F /Q Middlewares\Third_Party\LwIP\microej_readme.txt
    DEL  /F /Q Middlewares\Third_Party\mbedTLS\gpl-2.0.txt
) ELSE (
    ECHO Apply patch.
 )

REM Apply patch
CMD /C git apply CubeF7-MicroEJ.patch

REM Delete patch file.
DEL  /F /Q CubeF7-MicroEJ.patch

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM Restore current directory
CD %CURRENT_DIR%

exit /B 0
