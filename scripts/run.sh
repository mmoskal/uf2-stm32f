#!/bin/sh

set -e
make
(./scripts/bmp.sh && echo load && echo quit) > build/flash.gdb
arm-none-eabi-gdb --command=build/flash.gdb
