#!/bin/bash
cmake -DCMAKE_PREFIX_PATH="$(pwd)"/libtorch
cmake --build .