#!/bin/bash

set -xe

cd build
cmake ..

make test_socket
./tests/test_socket
