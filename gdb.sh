#!/bin/sh
arm-none-eabi-gdb --eval "target remote | openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -f debug.cfg" build/braingames_revb_bl/braingames_revb_bl.elf
