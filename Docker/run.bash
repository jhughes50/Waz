#!/bin/bash

xhost +
docker run --rm -it \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    --gpus all \
    -e "TERM=xterm-256color" \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -e XAUTHORITY=$XAUTH \
    --name waz-doc \
    waz-docker \
    bash
