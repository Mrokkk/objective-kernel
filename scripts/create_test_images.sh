#!/bin/bash

set -e

BASEDIR="$(dirname ${0})"

name="${1}"
multiboot_flag="${2}"

mkdir -p images
${BASEDIR}/isomaker.sh -i ${name} -o "images/test-image" ${multiboot_flag} --grub-use-serial --args "$(printf "%-64s" "")"

