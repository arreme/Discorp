#!/bin/sh
cmake -B build/Release -S . -DBUILD_TEST=ON -DDEBUG_MODE=ON
make -C build/Release -j 6