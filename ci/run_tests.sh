#!/bin/bash

set -e

dir=$(dirname $0)

cd $dir/..
mkdir -p build
cd build

cores=$(nproc)

cmake -DOPTIMIZE=${OPTIMIZE:-OFF} -DMULTIBOOT2=${MULTIBOOT2:-OFF} ..
make runtests -j$cores

