#!/bin/bash

docker run --rm -it --gpus all \
    --name manet-docker \
    -p 8115:8115 \
    -e "TERM=xterm-256color" \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -e XAUTHORITY=$XAUTH \
    -v "./data:/home/jasonah/data" \
    manet-doc \
    bash