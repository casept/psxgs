#!/usr/bin/env bash
set -eo

# Make the script callable from anywhere
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$DIR"

make clean
make -j"$(nproc)"

cd samples/cube
make clean
make -j"$(nproc)"
