FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gcc-aarch64-linux-gnu g++-aarch64-linux-gnu \
    openssl \
    libssl-dev \
    wget \
    # zlib1g zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

RUN wget https://www.openssl.org/source/openssl-1.1.1.tar.gz \
    && tar -xzf openssl-1.1.1.tar.gz \
    && cd openssl-1.1.1 \
    && ./Configure linux-aarch64 --prefix=/usr/local/arm-ssl --openssldir=/usr/local/arm-ssl shared \
    && make CC=aarch64-linux-gnu-gcc CXX=aarch64-linux-gnu-g++ \
    && make install \
    && cd .. \
    && rm -rf openssl-1.1.1.tar.gz openssl-1.1.1

WORKDIR /app

# ENV OPENSSL_ROOT_DIR=/usr/include/openssl/arm
