#!/bin/bash
set -e

openocd -f zybo-openocd.cfg \
        -f debug-ocd.cfg \
        -c "sleep 3000" \
        -c halt \
        -c "load_image app/build/embeddedhello.img 0x12100000" \
        -c resume
