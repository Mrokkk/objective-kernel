#!/bin/bash

set -e

dir=$(dirname $0)

cd $dir/..
mkdir -p build
cd build

cores=$(nproc)

if [[ ${UT} ]]; then
    cmake -DCOVERAGE=ON -DSANITIZERS=${SANITIZERS:-OFF} ..
    make ut-cov -j${cores}
else
    cmake -DOPTIMIZE=${OPTIMIZE:-OFF} -DMULTIBOOT2=${MULTIBOOT2:-OFF} ..
    make runtests -j$cores
fi
