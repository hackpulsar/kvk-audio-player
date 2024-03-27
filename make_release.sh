#!/usr/bin/env bash

cmake -DCMAKE_BUILD_TYPE=Release -B./build/release/
cd build/release/
make
cp compile_commands.json ../../build
