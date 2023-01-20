#!/bin/sh
cmake -B build/DisTest -S . -DBUILD_TEST=ON -DDEBUG_MODE=OFF
make -C build/DisTest