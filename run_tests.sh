#!/bin/bash

set -e

dir=$(dirname $0)

cd $dir
mkdir -p build
cd build

cores=$(nproc)

cmake -DCI_BUILD=ON ..
make runtests -j$cores

