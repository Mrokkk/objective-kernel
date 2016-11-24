#!/usr/bin/env bash

name=$1
mkdir -p ${name}.d/boot/grub

if [ ! -f ${name}.d/boot/grub/menu.lst ]; then
    echo default 0 > ${name}.d/boot/grub/menu.lst
    echo timeout 0 >> ${name}.d/boot/grub/menu.lst
    echo title My kernel >> ${name}.d/boot/grub/menu.lst
    echo kernel /kernel >> ${name}.d/boot/grub/menu.lst
    echo boot >> ${name}.d/boot/grub/menu.lst
fi

if [ ! -f stage2_eltorito ]; then
    echo Downloading bootloader stage2...
    wget -q ftp://ftp.free.org/mirrors/rsync.frugalware.org/frugalware-testing/boot/grub/stage2_eltorito
fi

cp stage2_eltorito ${name}.d/boot/grub/stage2_eltorito

cp ${name} ${name}.d/kernel
$(which genisoimage || which mkisofs) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o ${name}.iso ${name}.d 2> /dev/null

