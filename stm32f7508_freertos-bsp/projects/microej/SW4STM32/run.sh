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

# Keep current directory.
CURRENT_DIRECTORY=$(pwd)

# Set environment variables.
cd $(dirname $0)
. set_project_env.sh

"$GNU_TOOLS_FOR_STM32_DIR/arm-none-eabi-objcopy" -O ihex "$CURRENT_DIRECTORY/application.out" "$CURRENT_DIRECTORY/application.hex"
"$CUBE_PROGRAMMER_DIR/STM32_Programmer_CLI" -c port=SWD mode=UR -w "$CURRENT_DIRECTORY/application.hex" -el "$CUBEPROGRAMMER_DIR/ExternalLoader/N25Q128A_STM32F7508-DISCO.stldr" -rst

# Restore current directory.
cd $CURRENT_DIRECTORY