#!/bin/bash

set -xe

cd build

make test_webserver
./tests/test_webserver
