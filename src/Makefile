# DESCRIPTION: E93 OS Makefile
# AUTHOR: Erdem Ersoy (eersoy93)
# COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
# LICENSE: MIT license. See README.md file for deatils.

all: build

output/kernel.bin: kernel/kernel_entry.o kernel/kernel.o
	i686-linux-gnu-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel/kernel_entry.o: kernel/kernel_entry.asm
	nasm $< -f elf -o $@

kernel/kernel.o: kernel/kernel.c
	i686-linux-gnu-gcc -ffreestanding -fno-pie -c $< -o $@

output/loader.bin: loader/loader.asm
	nasm $< -f bin -o $@

output/e93-os.bin: output/loader.bin output/kernel.bin
	cat $^ > $@

build: output/e93-os.bin

run: output/e93-os.bin
	qemu-system-i386 -fda $<

clean:
	rm -f output/*.bin kernel/*.o
