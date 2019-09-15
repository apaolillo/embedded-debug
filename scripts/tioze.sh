#!/bin/bash
set -e

if [[ -z $1 ]]; then
    targetDevice=/dev/ttyUSB1
else
    targetDevice=$1
fi

tio \
  --baudrate 115200 \
  --databits 8 \
  --parity none \
  ${targetDevice}
