#!/bin/bash

set -xe


cd build
cmake ..

make test_epoll
./tests/test_epoll
