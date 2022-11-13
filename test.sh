#!/bin/sh
cmake -B build/DisTest -S . -DBUILD_TEST=ON
make -C build/DisTest