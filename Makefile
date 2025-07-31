ROOTFS_DIR = rootfs
ROOTFS_BIN_DIR = ${ROOTFS_DIR}/bin
ROOTFS_LIB_DIR = ${ROOTFS_DIR}/lib
ROOTFS_LIB64_DIR = ${ROOTFS_DIR}/lib64
ROOTFS_IMAGE = rootfs.cpio

FILES_SOURCE = $(wildcard src/*.c)
FILES_OBJECTS = $(FILES_SOURCE:.c=.o)
TARGET_INIT = ${ROOTFS_BIN_DIR}/init

SYSTEM_LIBS = /lib/x86_64-linux-gnu/libc.so.6
SYSTEM_LIBS64 = /lib64/ld-linux-x86-64.so.2
SYSTEM_KERNEL = /boot/vmlinuz-$(shell uname -r)

build: ${FILES_SOURCE}
	mkdir -p ${ROOTFS_DIR}/bin ${ROOTFS_DIR}/lib ${ROOTFS_DIR}/lib64 ${ROOTFS_DIR}/dev
	gcc -o ${TARGET_INIT} src/init.c src/main.c
	chmod +x ${TARGET_INIT}
	cp ${SYSTEM_LIBS} ${ROOTFS_LIB_DIR}/
	cp ${SYSTEM_LIBS64} ${ROOTFS_LIB64_DIR}/
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
	rm -rf ${ROOTFS_DIR} ${TARGET_INIT} ${ROOTFS_IMAGE}

