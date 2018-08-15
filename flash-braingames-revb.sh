#!/bin/sh
set -e
make -j10
openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program build/braingames_revb_bl/braingames_revb_bl.elf verify reset exit "
