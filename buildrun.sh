#!/bin/bash
set -e

IMAGE_TAG=jtagdebug
CONTAINER_NAME=embedded-debug

docker build -t $IMAGE_TAG .
docker run \
    --rm \
    -ti \
    --name $CONTAINER_NAME \
    --privileged -v /dev/bus/usb:/dev/bus/usb \
    --privileged -v /dev/ttyUSB0:/dev/ttyUSB0 \
    --privileged -v /dev/ttyUSB1:/dev/ttyUSB1 \
    $IMAGE_TAG \
    $@

# TODO check per-device privilege
