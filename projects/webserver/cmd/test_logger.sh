#!/bin/bash

set -xe

cd build

make test_webserver
./bin/test_webserver
