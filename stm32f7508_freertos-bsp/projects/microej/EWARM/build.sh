#!/bin/bash
#
# BASH
#
# Copyright 2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

# 'build.sh' implementation for Eclipse CDT.

# 'build.sh' is responsible for producing the executable file 
# then copying this executable file to the current directory where it has been executed to a file named 'application.out'

# Force to stop execution on error
set -e

# Save application current directory and jump one level above scripts
CURRENT_DIRECTORY=$(pwd)
SCRIPT_DIRECTORY="$(dirname $(realpath $0))"
cd $(dirname "$0")

echo "current location=$(pwd)"
. "set_project_env.sh"

# Apply the git patch
. "$SCRIPT_DIRECTORY/../scripts/patchSTM32CubeF7.sh"

cd $SCRIPT_DIRECTORY

"$IAREW_INSTALLATION_DIR/common/bin/iarbuild" "$IAREW_PROJECT_DIR/$IAREW_PROJECT_NAME.ewp" -make $IAREW_PROJECT_CONFIGURATION -log all

# Copy the generated .elf file
cp $IAREW_PROJECT_EXECUTABLE_FILE "$CURRENT_DIRECTORY/application.out"

# Restore current directory
cd "$CURRENT_DIRECTORY"
