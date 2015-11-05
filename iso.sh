#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/polo.kernel isodir/boot/polo.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "polo" {
	multiboot /boot/polo.kernel
}
EOF
grub-mkrescue -o polo.iso isodir
