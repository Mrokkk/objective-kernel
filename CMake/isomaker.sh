#!/usr/bin/env bash

set -e

base_dir=$(dirname $0)

while [[ $# -gt 0 ]]; do
    arg="$1"
    case $arg in
        -o|--output)
            name="$2"
            shift ;;
        --multiboot2)
            multiboot2=1 ;;
        *)
            break ;;
    esac
    shift
done

mkdir -p ${name}.d/boot/grub

if [ "$multiboot2" ]; then
    if [ ! -f ${name}.d/boot/grub/grub.cfg ]; then
        echo "set timeout=0
set default=0
menuentry "${name}" {
    multiboot2 /kernel
    boot
}" > ${name}.d/boot/grub/grub.cfg
    fi
    cp ${name} ${name}.d/kernel
    grub-mkrescue -o ${name}.iso ${name}.d 2> /dev/null
else
    if [ ! -f ${name}.d/boot/grub/menu.lst ]; then
        echo "default 0
timeout 0
title My kernel
kernel /kernel
boot" > ${name}.d/boot/grub/menu.lst
    fi
    cp $base_dir/stage2_eltorito ${name}.d/boot/grub/stage2_eltorito
    cp ${name} ${name}.d/kernel
    $(which genisoimage || which mkisofs) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o ${name}.iso ${name}.d 2> /dev/null
fi

