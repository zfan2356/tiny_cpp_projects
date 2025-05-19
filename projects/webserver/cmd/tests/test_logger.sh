#!/bin/bash

set -xe

cd build
cmake ..

make test_webserver
./tests/test_webserver
