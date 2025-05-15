#/bin/bash

mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd src
make VERBOSE=1
