#!/bin/sh
cmake -B build/Release -S . -DBUILD_TEST=OFF -DDEBUG_MODE=OFF
make -C build/Release -j 6