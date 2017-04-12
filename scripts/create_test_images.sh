#!/bin/bash

set -e

BASEDIR="$(dirname ${0})"

name="${1}"
multiboot_flag="${2}"

IFS=$'\n'
tests=(`strings ${name} | grep test_cases | c++filt | grep "()" | sed 's/test_cases:://g;s/__/\./g;s/()//g'`)
unset IFS

index=0
for t in "${tests[@]}"; do
    ${BASEDIR}/isomaker.sh -i ${name} -o "${index}-${t}.iso" ${multiboot_flag} --grub-use-serial --args "${t}"
    ((index+=1))
done

