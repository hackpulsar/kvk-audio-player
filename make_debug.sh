#!/usr/bin/env bash

cmake -DCMAKE_BUILD_TYPE=Debug -B./build/debug/
cd build/debug/
make
cp compile_commands.json ../../build
