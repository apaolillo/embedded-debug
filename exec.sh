#!/bin/bash
set -e

CONTAINER_NAME=embedded-debug

if [[ -z $@ ]]; then
    cmd=bash
else
    cmd=$@
fi

docker exec -ti $CONTAINER_NAME ${cmd}
