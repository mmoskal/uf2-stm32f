#!/bin/sh
arm-none-eabi-gdb --eval "target remote | openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -f debug.cfg" build/brainpad_bl/brainpad_bl.elf
