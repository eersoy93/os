#!/bin/sh
# DESCRIPTION: E93 OS build script
# AUTHOR: Erdem Ersoy (eersoy93)
# COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
# LICENSE: MIT license. See README.md file for deatils.

set -ex

echo "Building E93 OS..."

cd e93ldr

echo "Building E93LDR..."
nasm -f bin e93ldr.asm -o ../e93ldr.bin
echo "Finished building E93LDR!"

cd ..

cd kernel

echo "Building E93 Kernel..."
i686-linux-gnu-gcc -ffreestanding -fno-pie -c kernel.c -o kernel.o
i686-linux-gnu-ld -o kernel.bin -Ttext 0x0 --oformat binary kernel.o
echo "Finished building E93 Kernel!"

cd ..

echo "Finished building E93 OS!"

set +ex
