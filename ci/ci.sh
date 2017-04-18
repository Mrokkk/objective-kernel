#!/bin/bash

set -e

docker run -t -v $PWD:$PWD $IMAGE /bin/bash -c "OPTIMIZE=$OPTIMIZE MULTIBOOT2=$MULTIBOOT2 CXX=$COMPILER UT=$UT $PWD/ci/run_tests.sh"

