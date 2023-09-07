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

REM Get patch script directory
SET PATCH_SCRIPT_DIR=%~dp0

CD %PATCH_SCRIPT_DIR%\..\..\..\sdk\STM32CubeF7

setlocal EnableDelayedExpansion
REM Check if the patch was applied by looking for microej_readme.txt
IF NOT EXIST Middlewares\Third_Party\mbedTLS\microej_readme.txt (
    git --version > find_git.log 2>&1

    findstr /I /C:"git version" find_git.log > nul && (SET GIT_INSTALLED=1) || (SET GIT_INSTALLED=0)

    DEL /F /Q find_git.log

    IF !GIT_INSTALLED! EQU 0 (
        ECHO STM32CubeF7 installation is not patched
        ECHO Please install git and run this script again
        EXIT /B 1
    )

    ECHO Apply CubeF7-MicroEJ.patch to STM32CubeF7 submodule

    REM Copy the patch file to the submodule directory (STM32CubeF7)
    COPY /Y /Z /B "..\CubeF7_patch\CubeF7-MicroEJ.patch" "CubeF7-MicroEJ.patch"
    IF !ERRORLEVEL! NEQ 0 (
        EXIT /B !ERRORLEVEL!
    )

    CMD /C git apply CubeF7-MicroEJ.patch

    REM Delete patch file.
    DEL  /F /Q CubeF7-MicroEJ.patch

    IF !ERRORLEVEL! NEQ 0 (
        EXIT /B !ERRORLEVEL!
    )
) ELSE (
    ECHO CubeF7-MicroEJ.patch is already applied
)

endlocal 

REM Restore current directory
CD %CURRENT_DIR%

exit /B 0
