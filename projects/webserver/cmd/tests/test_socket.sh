#!/bin/bash

set -xe

if [ -d "build" ]; then
    rm -rf build
fi

bash cmd/build.sh

cd build
make test_socket
./tests/test_socket
