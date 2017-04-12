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
        --args)
            args=$2
            shift ;;
        *)
            break ;;
    esac
    shift
done

if [ "$multiboot2" ]; then
    multiboot_command="multiboot2"
else
    multiboot_command="multiboot"
fi

menu_entry="set timeout=0
set default=0
menuentry "${name}" {
    if ! $multiboot_command /kernel $args; then reboot; fi
    boot
}"

serial_set="serial --unit=0 --speed=9600
terminal_input serial
terminal_output serial"

mkdir -p ${name}.d/boot/grub

echo "$menu_entry" > ${name}.d/boot/grub/grub.cfg
if [ "$serial" ]; then
    echo "$serial_set" >> ${name}.d/boot/grub/grub.cfg
fi

cp ${name} ${name}.d/kernel
grub-mkrescue -o ${name}.iso ${name}.d 2> /dev/null

