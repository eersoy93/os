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
nasm kernel_entry.asm -f elf -o kernel_entry.o
i686-linux-gnu-ld -o ../kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary
echo "Finished building E93 Kernel!"

cd ..

echo "Building the E93 OS image..."

cat e93ldr.bin kernel.bin > e93os.bin

echo "Finished building E93 OS!"

set +ex
