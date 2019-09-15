#!/bin/bash
set -e

openocd -f ocdconfig/zybo-debug.cfg \
        -c "sleep 3000" \
        -c halt \
        -c "load_image app/build/embeddedhello.img 0x12100000" \
        -c resume
