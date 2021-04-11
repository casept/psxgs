#!/usr/bin/env sh
set -eo

make clean
make -j"$(nproc)"

cd samples/triangle
make clean
make -j"$(nproc)"
