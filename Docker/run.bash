#!/bin/bash

xhost +
docker run --rm -it \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v `pwd`/../CMakeLists.txt:/home/jason/CMakeLists.txt:rw \
    -v `pwd`/../include:/home/jason/include \
    -v `pwd`/../src:/home/jason/src \
    -v `pwd`/../cmake:/home/jason/cmake \
    --gpus all \
    -e "TERM=xterm-256color" \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -e XAUTHORITY=$XAUTH \
    --name waz-doc \
    waz-docker:dev \
    bash
