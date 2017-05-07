#!/usr/bin/env bash

set -e

base_dir=$(dirname $0)
modules=()

while [[ $# -gt 0 ]]; do
    arg="$1"
    case $arg in
        -i|--input)
            binary="$2"
            shift ;;
        -o|--output)
            name="$2"
            shift ;;
        --multiboot2)
            multiboot2=1 ;;
        --grub-use-serial)
            serial=1 ;;
        --args)
            args="$2"
            shift ;;
        --module)
            modules+=("$(readlink -e ${2})")
            shift ;;
        *)
            break ;;
    esac
    shift
done

if [[ "${binary}" == "" ]]; then
    binary="${name}"
fi

if [[ "${multiboot2}" ]]; then
    multiboot_command="multiboot2"
else
    multiboot_command="multiboot"
fi

modules_load=""
for mod in "${modules[@]}"; do
    modules_load+="module /$(basename ${mod}) $(basename ${mod})
    "
done

# For debug: set debug=all
menu_entry="set timeout=0
set default=0
menuentry "${binary}" {
    if ! ${multiboot_command} /kernel ${args}; then reboot; fi
    ${modules_load}
    boot
}"

serial_set="serial --unit=0 --speed=9600
terminal_input serial
terminal_output serial"

mkdir -p ${name}.d/boot/grub

echo "${menu_entry}" >${name}.d/boot/grub/grub.cfg
if [ "${serial}" ]; then
    echo "${serial_set}" >>${name}.d/boot/grub/grub.cfg
fi

for mod in "${modules[@]}"; do
    cp ${mod} ${name}.d
done

cp ${binary} ${name}.d/kernel
grub-mkrescue -o ${name}.iso ${name}.d 2>/dev/null

