#!/bin/bash

set -xe

cd build
cmake ..
make test_cpp

./tests/test_cpp
