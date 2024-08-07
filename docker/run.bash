#!/bin/bash

xhost +
docker run --rm -it \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v `pwd`/../CMakeLists.txt:/home/jason/ws/src/waz/CMakeLists.txt:rw \
    -v `pwd`/../package.xml:/home/jason/ws/src/waz/package.xml:rw \
    -v `pwd`/../include:/home/jason/ws/src/waz/include \
    -v `pwd`/../ros:/home/jason/ws/src/waz/ros \
    -v `pwd`/../src:/home/jason/ws/src/waz/src \
    -v `pwd`/../cmake:/home/jason/ws/src/waz/cmake \
    -v `pwd`/../test:/home/jason/ws/src/waz/test \
    -v `pwd`/../config:/home/jason/ws/src/waz/config \
    -v `pwd`/../models:/home/jason/ws/src/waz/models \
    -v `pwd`/../utils:/home/jason/ws/src/waz/utils \
    -v `pwd`/../launch:/home/jason/ws/src/waz/launch \
    -v `pwd`/../.waz:/home/jason/ws/src/waz/.waz \
    -v /home/jason/ROS/bags/:/home/jason/bags \
    --gpus all \
    -e "TERM=xterm-256color" \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -e XAUTHORITY=$XAUTH \
    --name waz-doc \
    waz-docker:ros \
    bash
