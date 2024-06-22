#!/bin/bash

docker run --rm -it --gpus '"device=0"' \
    --name manet-docker \
    -p 8115:8115 \
    -e "TERM=xterm-256color" \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -e XAUTHORITY=$XAUTH \
    -v "./models:/home/jasonah/models" \
    -v "./notebooks:/home/jasonah/notebooks" \
    -v "./scripts:/home/jasonah/scripts" \
    -v "/pool/jasonah/WestPoint:/home/jasonah/data" \
    -v "/pool/jasonah/Manet/depth-anything/checkpoints/:/home/jasonah/models/Depth-Anything-V2/depth_anything_v2/checkpoints" \
    manet-doc \
    bash
