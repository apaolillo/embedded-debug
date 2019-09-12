#!/bin/bash
set -e

IMAGE_TAG=jtagdebug
docker build -t $IMAGE_TAG .
docker run \
    --rm \
    -ti \
    --name debug-container \
    --privileged -v /dev/bus/usb:/dev/bus/usb \
    $IMAGE_TAG
# TODO check if not too much (mounting usb volume), how did we do for hipp?
