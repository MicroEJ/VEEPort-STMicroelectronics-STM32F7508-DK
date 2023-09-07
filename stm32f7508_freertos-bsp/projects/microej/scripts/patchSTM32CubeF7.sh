#!/bin/sh
#
# bash
#
# Copyright 2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

# This script will apply patch (CubeF7-MicroEJ.patch) to STM32CubeF7 submodule

# Force to stop execution on error
set -e

# Get current directory
CURRENT_DIR=$(pwd)
echo CURRENT_DIR=$CURRENT_DIR

STM32CUBEF7_BSP_PATH=$(dirname $(realpath $0))/../../../sdk/STM32CubeF7
echo "STM32CUBEF7_BSP_PATH=$STM32CUBEF7_BSP_PATH"

# Go to STM32CUBEF7 BSP location
cd $STM32CUBEF7_BSP_PATH

if ! [[ -f "Middlewares/Third_Party/mbedTLS/microej_readme.txt" ]]; then
	git_version=$(git --version 2>&1)
	
	echo "Git version: $git_version"
	
	if ! [[ $git_version == "git version"* ]]; then
        echo "STM32CubeF7 installation is not patched"
        echo "Please install git and run this script again"
        exit 1
	fi
	
	echo "Apply CubeF7-MicroEJ.patch to STM32CubeF7 submodule"
	
	# Copy the patch file to the submodule directory (STM32CubeF7)
	cp "../CubeF7_patch/CubeF7-MicroEJ.patch" "./CubeF7-MicroEJ.patch"
	git apply -v CubeF7-MicroEJ.patch

    # Delete patch file
    rm CubeF7-MicroEJ.patch
else 
	echo "CubeF7-MicroEJ.patch is already applied"
fi

# Restore current directory
cd $CURRENT_DIR


