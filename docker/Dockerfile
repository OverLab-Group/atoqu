FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \\
    build-essential cmake git python3 python3-pip \\
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN mkdir -p build && cd build && \\
    cmake -DATOQUENABLECUDA=OFF .. && \\
    cmake --build .

EXPOSE 8080
CMD ["./build/atoqu", "--http", "8080"]
