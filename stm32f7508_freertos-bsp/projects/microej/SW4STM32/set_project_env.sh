#!/bin/bash
#
# BASH
#
# Copyright 2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

# 'set_project_env.bat' implementation for ECLIPSE_CDT.

# 'set_project_env' is responsible for
# - checking the availability of required environment variables 
# - setting project local variables for 'build.bat' and 'run.bat' 

SCRIPT_PATH=$(dirname "$0")

LOCAL_ENV_SCRIPT="$SCRIPT_PATH/set_local_env.sh"
if [[ -e "$LOCAL_ENV_SCRIPT" ]]; then
    echo "Load $LOCAL_ENV_SCRIPT"
    . "$LOCAL_ENV_SCRIPT"
fi

# Set the project variables required by the external build script for Eclipse CDT.
export ECLIPSE_CDT_INSTALLATION_DIR=/opt/st/stm32cubeide_1.9.0
export ECLIPSE_CDT_EXECUTABLE=stm32cubeide
export ECLIPSE_CDT_PROJECT_DIR=$SCRIPT_PATH
export ECLIPSE_CDT_PROJECT_NAME=application
export ECLIPSE_CDT_PROJECT_CONFIGURATION=Release
export ECLIPSE_CDT_WORKSPACE_DIR=$SCRIPT_PATH/../cdt_workspace

# The Cube Programmer utility directory absolute path
export CUBE_PROGRAMMER_DIR="$ECLIPSE_CDT_INSTALLATION_DIR/plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.linux64_2.0.200.202202231230/tools/bin"

# The GNU toolchain arm-none-eabi-objcopy directory absolute path
export GNU_TOOLS_FOR_STM32_DIR="$ECLIPSE_CDT_INSTALLATION_DIR/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.linux64_1.0.0.202111181127/tools/bin"

echo "ECLIPSE_CDT_INSTALLATION_DIR=$ECLIPSE_CDT_INSTALLATION_DIR"
echo "ECLIPSE_CDT_PROJECT_DIR=$ECLIPSE_CDT_PROJECT_DIR"
echo "ECLIPSE_CDT_PROJECT_NAME=$ECLIPSE_CDT_PROJECT_NAME"
echo "ECLIPSE_CDT_PROJECT_CONFIGURATION=$ECLIPSE_CDT_PROJECT_CONFIGURATION"
echo "CUBE_PROGRAMMER_DIR=$CUBE_PROGRAMMER_DIR"
