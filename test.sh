#!/bin/sh
cmake -B build/DisTest -S . -DBUILD_TEST=ON -DDEBUG_MODE=ON
make -C build/DisTest