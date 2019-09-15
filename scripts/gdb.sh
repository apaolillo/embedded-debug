#!/bin/bash
set -e

arm-none-eabi-gdb -x zybo-gdb.script app/build/embeddedhello
