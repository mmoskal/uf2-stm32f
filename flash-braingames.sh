#!/bin/sh
set -e
make -j10
openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program build/braingames_bl/braingames_bl.elf verify reset exit "
