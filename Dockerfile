FROM continuumio/miniconda3:latest AS miniconda
FROM nvidia/cudagl:11.4.2-base-ubuntu20.04

LABEL maintainer="Jason Hughes"

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
# system depends
RUN apt-get install -y --no-install-recommends gcc cmake sudo
RUN apt-get update && apt-get install -y --no-install-recommends python3-pip git python3-dev python3-opencv libglib2.0.0 wget python3-pybind11 vim tmux
RUN apt-get install -y --no-install-recommends libjsoncpp-dev libepoxy-dev libglm-dev libosmesa6 libosmesa6-dev libglew-dev libopencv-dev python3-pip

# Add user
ARG GID=1011
ARG UID=1011
ENV USER jasonah
RUN addgroup --gid $GID $USER 
RUN useradd --system --create-home --shell /bin/bash --groups sudo -p "$(openssl passwd -1 ${USER})" --uid $UID --gid $GID $USER
WORKDIR /home/$USER

# setup conda
COPY --from=miniconda /opt/conda /opt/conda
RUN chown -R $USER: /opt/conda 
RUN chown -R $USER: /home/$USER
USER $USER

# conda install
RUN echo ". /opt/conda/etc/profile.d/conda.sh" >> /home/$USER/.bashrc && \
    echo "conda activate base" >> /home/$USER/.bashrc
SHELL ["/bin/bash", "-c"]

RUN source /opt/conda/etc/profile.d/conda.sh \
 && conda init bash \
 && conda create -n manet -y python=3.9 cmake=3.14.0 \
 && conda activate manet \
 && conda install pytorch torchvision torchaudio pytorch-cuda=12.1 -c pytorch -c nvidia -y \
 && conda install conda-forge::transformers -y \
 && conda install anaconda::numpy -y \
 && conda install conda-forge::matplotlib -y \
 && conda install anaconda::jupyter -y \
 && conda install anaconda::notebook -y \
 && pip3 install --upgrade notebook

RUN mkdir ~/data
RUN mkdir ~/models
RUN mkdir ~/notebooks
RUN mkdir ~/scripts

RUN echo 'export PS1="\[$(tput setaf 2; tput bold)\]\u\[$(tput setaf 7)\]@\[$(tput setaf 3)\]\h\[$(tput setaf 7)\]:\[$(tput setaf 4)\]\W\[$(tput setaf 7)\]$ \[$(tput sgr0)\]"' >> /home/$USER/.bashrc
RUN echo "conda activate manet" >> /home/$USER/.bashrc

WORKDIR /home/$USER
CMD ["bash"]
