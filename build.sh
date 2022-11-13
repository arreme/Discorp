#!/bin/sh
cmake -B build/Discorp -S . -DBUILD_TEST=OFF
make -C build/Discorp