#!/bin/bash
#
# BASH
#
# Copyright 2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

# 'set_project_env.bat' implementation for IAR BXARM.

# 'set_project_env' is responsible for
# - checking the availability of required environment variables 
# - setting project local variables for 'build.bat' and 'run.bat' 

SCRIPT_PATH=$(dirname "$0")

LOCAL_ENV_SCRIPT="$SCRIPT_PATH/set_local_env.sh"
if [[ -e "$LOCAL_ENV_SCRIPT" ]]; then
    echo "Load $LOCAL_ENV_SCRIPT"
    . "$LOCAL_ENV_SCRIPT"
fi

# Required Environment Variable
# IAR Embedded Workbench installation directory (e.g.: /opt/iarsystems/bxarm)
export IAREW_INSTALLATION_DIR=/opt/iarsystems/bxarm

# Set the project variables required by the build script for IAR Embedded Workbench
# IAREW project directory that contains the project file .ewp (e.g.: $(dirname "$0"))
export IAREW_PROJECT_DIR=$SCRIPT_PATH
# IAREW project file name without the extension .ewp (e.g.: Project)
export IAREW_PROJECT_NAME=application
# IAREW project configuration (e.g.: Debug or Release)
export IAREW_PROJECT_CONFIGURATION=Release

export IAREW_PROJECT_EXECUTABLE_FILE="$IAREW_PROJECT_DIR/$IAREW_PROJECT_CONFIGURATION/$IAREW_PROJECT_NAME.out"

echo IAREW_INSTALLATION_DIR=$IAREW_INSTALLATION_DIR
echo IAREW_PROJECT_DIR=$IAREW_PROJECT_DIR
echo IAREW_PROJECT_NAME=$IAREW_PROJECT_NAME
echo IAREW_PROJECT_CONFIGURATION=$IAREW_PROJECT_CONFIGURATION