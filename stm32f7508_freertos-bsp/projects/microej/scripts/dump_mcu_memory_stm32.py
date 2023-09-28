#!/usr/bin/env python3

#
# Python
#
# Copyright 2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

from pathlib import Path
import os
import argparse
import shutil
import sys


###############################################################################
# START OF USER CUSTOMIZATION AREA

DEFAULT_START_ADDRESS = "0x20010000"
DEFAULT_MEMORY_SIZE = "0x400"
DEFAULT_OUTPUT_FILE_FULLPATH=Path("./output/output.bin")

# END OF USER CUSTOMIZATION AREA
###############################################################################

print("=== DUMP MCU Memory STM32 ===")
print("Requirements:")
print("STM32CubeProgrammer v2.13.0 or higher")
print("The Path of STM32_Programmer_CLI_INSTALL_DIR should be something like \"C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\\bin\" ")
print("=====")

# Check availability of STM32_Programmer_CLI.exe
cmds = [("STM32_Programmer_CLI.exe", "STM32_Programmer_CLI_INSTALL_DIR")]

cmds_exe = [None]*len(cmds)
for i, (cmd, env_path) in enumerate(cmds):
    cmds_exe[i] = shutil.which(cmd)
    if None == cmds_exe[i] and env_path in os.environ.keys():
        cmds_exe[i] = shutil.which(cmd, os.F_OK, os.environ.get(env_path))
    if None == cmds_exe[i]:
        print("-E- Could not find %s. Update PATH environment variable or create a %s environment variable" % (cmd, env_path))
        sys.exit(-1)
    print("-I- %s: %s" % (cmd, cmds_exe[i]))

memdump = cmds_exe[0]


# read arguments
parser = argparse.ArgumentParser()

parser.add_argument("-a", "--start_addr", default=DEFAULT_START_ADDRESS, help="The start address of the memory to retrieve")
parser.add_argument("-s", "--size", default=DEFAULT_MEMORY_SIZE, help="The amount of memory to retrieve")
parser.add_argument("-o", "--output", type=Path, default=DEFAULT_OUTPUT_FILE_FULLPATH, help="Output file fullpath")

args = parser.parse_args()

output_path = Path(args.output).parent

if not os.path.exists(output_path):
    os.makedirs(output_path)
    print("-I- Output directory created: " + str(output_path))

cmd =  '"%s"' % memdump + " -c port=SWD mode=HOTPLUG -u 0x%x 0x%x %s" % (int(args.start_addr, 16), int(args.size, 16), args.output)
print("-I- cmd:" + cmd)
os.system(cmd)

print("=== END ===")
