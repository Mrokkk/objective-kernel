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
        --grub-use-serial)
            serial=1 ;;
        *)
            break ;;
    esac
    shift
done

multiboot1_entry1="default 0
timeout 0
title ${name}"

multiboot1_entry2="kernel /kernel
boot"

multiboot1_serial_set="serial --unit=0 --speed=9600
terminal serial"

multiboot2_entry="set timeout=0
set default=0
menuentry "${name}" {
    if ! multiboot2 /kernel; then reboot; fi
    boot
}"

multiboo2_serial_set="serial --unit=0 --speed=9600
terminal_input serial
terminal_output serial"

mkdir -p ${name}.d/boot/grub

if [ "$multiboot2" ]; then
    echo "$multiboot2_entry" > ${name}.d/boot/grub/grub.cfg
    if [ "$serial" ]; then
        echo "$multiboo2_serial_set" >> ${name}.d/boot/grub/grub.cfg
    fi
    cp ${name} ${name}.d/kernel
    grub-mkrescue -o ${name}.iso ${name}.d 2> /dev/null
else
    echo "$multiboot1_entry1" > ${name}.d/boot/grub/menu.lst
    if [ "$serial" ]; then
        echo "$multiboot1_serial_set" >> ${name}.d/boot/grub/menu.lst
    fi
    echo "$multiboot1_entry2" >> ${name}.d/boot/grub/menu.lst
    cp $base_dir/stage2_eltorito ${name}.d/boot/grub/stage2_eltorito
    cp ${name} ${name}.d/kernel
    $(which genisoimage || which mkisofs) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o ${name}.iso ${name}.d 2> /dev/null
fi

