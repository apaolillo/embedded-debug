#!/bin/bash
set -e

targetDevice=$1

tio \
  --baudrate 115200 \
  --databits 8 \
  --parity none \
  ${targetDevice}
