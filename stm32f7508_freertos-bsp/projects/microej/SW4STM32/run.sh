#!/bin/bash
#
# BASH
#
# Copyright 2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

# 'run.sh' implementation for Espressif IDF.

# 'run.sh' is responsible for flashing the executable file on the target device 
# then resetting target device

# Force to stop execution on error
set -e

# Set environment variables.
cd $(dirname $0)
. set_project_env.sh

if [ -z "$1" ]; then
    APPLICATION_FILE="$(pwd)/application.out"
else
    APPLICATION_FILE="$(realpath "$1")"
fi

if [ ! -e "$APPLICATION_FILE" ]; then
    echo "FAILED - file '$APPLICATION_FILE' does not exist"
    exit 1
fi

"$GNU_TOOLS_FOR_STM32_DIR/arm-none-eabi-objcopy" -O ihex "${APPLICATION_FILE}" "${APPLICATION_FILE}.hex"
"$CUBE_PROGRAMMER_DIR/STM32_Programmer_CLI" -c port=SWD mode=UR -w "${APPLICATION_FILE}.hex" -el "$CUBE_PROGRAMMER_DIR/ExternalLoader/N25Q128A_STM32F7508-DISCO.stldr" -rst

# Restore current directory.
cd $CURRENT_DIRECTORY
