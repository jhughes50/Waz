# Manet Dev

Working on planning in the image space with semantic and relative depth priors.

### Docker Setup

To use jupyer in docker run the following commands:
1. On your local machine run: `ssh -L 8088:localhost:8115 jasonah@*.*.*.*`
2. Once on the server start the docker image with: `./run.bash`
3. Start the jupyer notebook with: `jupyter notebook --ip 0.0.0.0 --port 8115 --allow-root`
4. Now copy and paste the address and change the port number from `8115` to `8088`
