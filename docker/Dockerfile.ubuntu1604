FROM ubuntu:16.04
MAINTAINER   Andrey Bronin <jonnib@yandex.ru>
CMD          bash

RUN apt-get update && \
apt-get install -y --no-install-recommends \
apt-utils \
ca-certificates \
build-essential \
git \
llvm-dev \
make \
cmake \
libclang-5.0-dev \
llvm-5.0-dev \
libboost-dev \
libboost-iostreams-dev


RUN cd /home && git clone --depth=1 https://github.com/google/googletest.git &&\ 
cd googletest && cmake . && make install -j5

