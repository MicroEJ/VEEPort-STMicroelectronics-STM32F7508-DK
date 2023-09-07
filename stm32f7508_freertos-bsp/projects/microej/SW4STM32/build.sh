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

"$ECLIPSE_CDT_INSTALLATION_DIR/$ECLIPSE_CDT_EXECUTABLE" --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "$ECLIPSE_CDT_WORKSPACE_DIR" -import "."

"$ECLIPSE_CDT_INSTALLATION_DIR/$ECLIPSE_CDT_EXECUTABLE" --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "$ECLIPSE_CDT_WORKSPACE_DIR" -build "$ECLIPSE_CDT_PROJECT_NAME/$ECLIPSE_CDT_PROJECT_CONFIGURATION"

# Copy the generated .out file
cp "$ECLIPSE_CDT_PROJECT_DIR/$ECLIPSE_CDT_PROJECT_CONFIGURATION/$ECLIPSE_CDT_PROJECT_NAME.elf" "$CURRENT_DIRECTORY/application.out"

cd "$CURRENT_DIRECTORY"