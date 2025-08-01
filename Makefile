ROOTFS_DIR = rootfs
ROOTFS_BIN_DIR = ${ROOTFS_DIR}/bin
ROOTFS_DEV_DIR = ${ROOTFS_DIR}/dev
ROOTFS_IMAGE = rootfs.cpio

FILES_INIT_SOURCE = init/init.c init/main.c init/gfx.c
FILES_INIT_OBJECTS = $(FILES_INIT_SOURCE:.c=.o)
TARGET_INIT = ${ROOTFS_BIN_DIR}/init

FILES_POWEROFF_SOURCE = init/poweroff.c
FILES_POWEROFF_OBJECTS = $(FILES_POWEROFF_SOURCE:.c=.o)
TARGET_POWEROFF = ${ROOTFS_BIN_DIR}/poweroff

FILES_OBJECTS = ${FILES_INIT_OBJECTS} ${FILES_POWEROFF_OBJECTS}
SYSTEM_KERNEL = /boot/vmlinuz-$(shell uname -r)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c17 -D_GNU_SOURCE
LDFLAGS = -static


# Pattern rule to build .o files from .c files
src/%.o: src/%.c
	${CC} ${CFLAGS} -c $< -o $@

build: ${FILES_OBJECTS}
	# Create the root filesystem directory structure
	mkdir -p ${ROOTFS_BIN_DIR} ${ROOTFS_DEV_DIR}
	${CC} ${CFLAGS} -o ${TARGET_INIT} ${FILES_INIT_OBJECTS} ${LDFLAGS}
	${CC} ${CFLAGS} -o ${TARGET_POWEROFF} ${FILES_POWEROFF_OBJECTS} ${LDFLAGS}
	# Ensure the init binary is executable
	chmod +x ${TARGET_INIT}
	# Create framebuffer device node (major 29, minor 0)
	@if ! sudo mknod -m 666 ${ROOTFS_DIR}/dev/fb0 c 29 0 2>/dev/null; then \
		echo "Warning: Could not create /dev/fb0 (need sudo)! Run 'sudo mknod -m 666 ${ROOTFS_DIR}/dev/fb0 c 29 0' manually if needed."; \
	fi
	# Create tty device (major 4, minors 0)
	@if ! sudo mknod -m 666 ${ROOTFS_DIR}/dev/tty0 c 4 0 2>/dev/null; then \
		echo "Warning: Could not create /dev/tty0 (need sudo)! Run 'sudo mknod -m 666 ${ROOTFS_DIR}/dev/tty0 c 4 0' manually if needed."; \
	fi
	# Create null device (major 1, minor 3)
	@if ! sudo mknod -m 666 ${ROOTFS_DIR}/dev/null c 1 3 2>/dev/null; then \
		echo "Warning: Could not create /dev/null (need sudo)! Run 'sudo mknod -m 666 ${ROOTFS_DIR}/dev/null c 1 3' manually if needed."; \
	fi
	cd ${ROOTFS_DIR} && find . | cpio -o --format=newc > ../${ROOTFS_IMAGE}

run: build
	sudo qemu-system-x86_64 -kernel ${SYSTEM_KERNEL} -initrd ${ROOTFS_IMAGE} -append "root=/dev/ram rdinit=/bin/init vga=789 quiet" -m 512 -vga std

clean:
	rm -rf ${ROOTFS_DIR} ${TARGET_INIT} ${ROOTFS_IMAGE} ${FILES_OBJECTS}

.PHONY: build run clean
