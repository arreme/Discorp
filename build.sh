#!/bin/sh
cmake -B build/Disland -S . -DBUILD_TEST=OFF
make -C build/Disland -j 6