#!/bin/bash

set -xe

if [ -d "build" ]; then
    rm -rf build
fi

export PATH="$HOME/bin:$PATH"

eval "$(micromamba shell hook --shell bash)"
micromamba activate clang-env

cmake -B build -DCONDA_PREFIX="$CONDA_PREFIX" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
