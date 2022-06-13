# DESCRIPTION: E93 OS Makefile
# AUTHOR: Erdem Ersoy (eersoy93)
# COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
# LICENSE: MIT license. See README.md file for deatils.

ASM_SOURCES = $(wildcard loader/*.asm)
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o}

CC = /usr/bin/i686-linux-gnu-gcc
CFLAGS = -fno-pie

e93-os.bin: loader.bin kernel.bin
	cat $^ > $@

kernel.bin: kernel/kernel_entry.o ${OBJ}
	i686-linux-gnu-ld -o $@ -Ttext 0x1000 $^ --oformat binary

run: e93-os.bin
	qemu-system-i386 -fda $<

%.o: %.c ${C_HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: ${ASM_SOURCES}
	nasm $< -f bin -o $@

clean:
	rm -f *.bin *.o boot/*.bin kernel/*.o loader/*.o drivers/*.o
