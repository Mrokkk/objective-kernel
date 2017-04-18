#!/bin/bash

set -e

dir=$(dirname $0)

cd $dir/..
mkdir -p build
cd build

cores=$(nproc)

if [[ ${UT} ]]; then
    cmake ..
    make ut-run -j${cores}
else
    cmake -DOPTIMIZE=${OPTIMIZE:-OFF} -DMULTIBOOT2=${MULTIBOOT2:-OFF} ..
    make runtests -j$cores
fi
