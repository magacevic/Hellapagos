FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    qt6-base-dev \
    qt6-multimedia-dev \
    libgl1-mesa-dev \
    libxkbcommon-dev \
    libxcb-cursor0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# Uklanjanje eventualnog postojećeg build direktorijuma
RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make
