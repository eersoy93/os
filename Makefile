ROOTFS_DIR = rootfs
ROOTFS_BIN_DIR = ${ROOTFS_DIR}/bin
ROOTFS_LIB_DIR = ${ROOTFS_DIR}/lib
ROOTFS_LIB64_DIR = ${ROOTFS_DIR}/lib64
ROOTFS_IMAGE = rootfs.cpio

FILES_SOURCE = $(wildcard src/*.c)
TARGET_INIT = ${ROOTFS_BIN_DIR}/init

SYSTEM_LIBS = /lib/x86_64-linux-gnu/libc.so.6
SYSTEM_LIBS64 = /lib64/ld-linux-x86-64.so.2
SYSTEM_KERNEL = /boot/vmlinuz-$(shell uname -r)

build: ${FILES_SOURCE}
	mkdir -p ${ROOTFS_DIR}/bin ${ROOTFS_DIR}/lib ${ROOTFS_DIR}/lib64
	gcc -o ${TARGET_INIT} ${FILES_SOURCE}
	chmod +x ${TARGET_INIT}
	cp ${SYSTEM_LIBS} ${ROOTFS_LIB_DIR}/
	cp ${SYSTEM_LIBS64} ${ROOTFS_LIB64_DIR}/
	cd ${ROOTFS_DIR} && find . | cpio -o --format=newc > ../${ROOTFS_IMAGE}

run: build
	sudo qemu-system-x86_64 -kernel ${SYSTEM_KERNEL} -initrd ${ROOTFS_IMAGE} -append "root=/dev/ram rdinit=/bin/init" -m 512

clean:
	rm -rf ${ROOTFS_DIR} ${TARGET_INIT} ${ROOTFS_IMAGE}
	
