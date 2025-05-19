#!/bin/bash

set -xe

cd build
make test_cpp

./tests/test_cpp
