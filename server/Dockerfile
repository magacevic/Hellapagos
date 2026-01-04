FROM ubuntu:22.04

# Instalacija potrebnih paketa
RUN apt-get update && apt-get install -y \
    qt6-base-dev \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Radni direktorijum u kontejneru
WORKDIR /app

# Kopiranje izvornog koda (bez lokalnog build-a)
COPY . .

# Uklanjanje eventualnog postojećeg build direktorijuma
RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

# Pokretanje server aplikacije
CMD ["./build/server"]

