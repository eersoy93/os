# DESCRIPTION: E93 OS Makefile
# AUTHOR: Erdem Ersoy (eersoy93)
# COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
# LICENSE: MIT license. See README.md file for deatils.

C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)

OBJ = ${C_SOURCES:.c=.o cpu/interrupts.o}

CC = /usr/bin/i686-linux-gnu-gcc
CFLAGS = -fno-pie

e93-os.bin: loader/loader.bin kernel.bin
	cat $^ > $@

kernel.bin: loader/kernel_entry.o ${OBJ}
	i686-linux-gnu-ld -o $@ -Ttext 0x1000 $^ --oformat binary --allow-multiple-definition

run: e93-os.bin
	qemu-system-i386 -fda $<

%.o: %.c ${C_HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -f *.bin *.o loader/*.bin kernel/*.o loader/*.o drivers/*.o cpu/*.o
