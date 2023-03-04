#!/bin/sh
cmake -B build/test -S . -DBUILD_TESTS=ON -DDEBUG_MODE=OFF
make -C build/test -j 6