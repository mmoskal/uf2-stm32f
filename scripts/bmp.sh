#!/bin/sh

d=$(ls -1 /dev/cu.usbmodem* | head -1)
cat <<EOF
file build/STM32
target extended-remote $d
monitor swdp_scan
attach 1
EOF
