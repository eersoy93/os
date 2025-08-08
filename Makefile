ROOTFS_DIR = rootfs
ROOTFS_BIN_DIR = ${ROOTFS_DIR}/bin
ROOTFS_DEV_DIR = ${ROOTFS_DIR}/dev
ROOTFS_USR_DIR = ${ROOTFS_DIR}/usr
ROOTFS_IMAGE = rootfs.cpio

FILES_LIB_SOURCE = $(wildcard lib/*.c)
FILES_LIB_OBJECTS = $(FILES_LIB_SOURCE:.c=.o)

FILES_INIT_SOURCE = init/init.c init/main.c
FILES_INIT_OBJECTS = $(FILES_INIT_SOURCE:.c=.o)
TARGET_INIT = ${ROOTFS_BIN_DIR}/init

FILES_POWEROFF_SOURCE = apps/poweroff/poweroff.c
FILES_POWEROFF_OBJECTS = $(FILES_POWEROFF_SOURCE:.c=.o)
TARGET_POWEROFF = ${ROOTFS_BIN_DIR}/poweroff
TARGET_FONT = ${ROOTFS_USR_DIR}/font.ttf

FILES_OBJECTS = ${FILES_LIB_OBJECTS} ${FILES_INIT_OBJECTS} ${FILES_POWEROFF_OBJECTS}
SYSTEM_KERNEL = /boot/vmlinuz-$(shell uname -r)
SYSTEM_FONT = /usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c17 -D_GNU_SOURCE -I./include -I/usr/include/freetype2 -I/usr/include/libpng16

# Use -static to ensure the binaries are statically linked
LDFLAGS = -static

# FreeType flags (prefer pkg-config)
FREETYPE_CFLAGS := $(shell pkg-config --cflags freetype2 2>/dev/null)
FREETYPE_LIBS := $(shell pkg-config --libs --static freetype2 2>/dev/null)
ifneq ($(strip ${FREETYPE_CFLAGS}),)
	CFLAGS += ${FREETYPE_CFLAGS}
endif
ifeq ($(strip ${FREETYPE_LIBS}),)
	# Fallback if pkg-config isn't available: common static deps on Debian
	FREETYPE_LIBS := -lfreetype -lbz2 -lz -lpng16 -lbrotlidec -lbrotlicommon -lm
endif


# Pattern rule to build .o files from .c files
src/%.o: src/%.c
	${CC} ${CFLAGS} -c $< -o $@

build: ${FILES_OBJECTS}
	# Create the root filesystem directory structure
	mkdir -p ${ROOTFS_BIN_DIR} ${ROOTFS_DEV_DIR} ${ROOTFS_USR_DIR}
	${CC} ${CFLAGS} -o ${TARGET_INIT} ${FILES_LIB_OBJECTS} ${FILES_INIT_OBJECTS} ${LDFLAGS} ${FREETYPE_LIBS}
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
	# Copy the system font to the root filesystem
	@if [ -f ${SYSTEM_FONT} ]; then \
		sudo cp ${SYSTEM_FONT} ${TARGET_FONT}; \
	else \
		echo "Warning: System font not found at ${SYSTEM_FONT}. Font rendering may not work."; \
	fi
	# Copy the files to the root filesystem
	cd ${ROOTFS_DIR} && find . | cpio -o --format=newc > ../${ROOTFS_IMAGE}

run: build
	sudo qemu-system-x86_64 -kernel ${SYSTEM_KERNEL} -initrd ${ROOTFS_IMAGE} -append "root=/dev/ram rdinit=/bin/init vga=789 quiet" -m 512 -vga std

clean:
	rm -rf ${ROOTFS_DIR} ${TARGET_INIT} ${ROOTFS_IMAGE} ${FILES_OBJECTS}

.PHONY: build run clean
