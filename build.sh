#!/bin/sh
# DESCRIPTION: E93 OS build script
# AUTHOR: Erdem Ersoy (eersoy93)
# COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
# LICENSE: MIT license. See README.md file for deatils.

set -ex

echo "Building E93 OS..."

nasm -f bin e93ldr.asm -o e93ldr.bin

echo "Finished building E93 OS!"

set +ex
