#!/bin/sh
openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program build/brainpad_bl/brainpad_bl.elf verify reset exit "
