# Facepass

A FaceID login module for Linux.

## Why?

There's also a FaceID login module for Linux: [Howdy](https://github.com/boltgolt/howdy). However, Howdy is not actively maintained. Last release is on Sep 2, 2020 and many dependencies are deprecated. Howdy also cannot recognize spoofed faces. That's why I decided to create Facepass.

## Architecture

COMING SOON

## How to compile

1. Install dependencies
    ```bash
    sudo apt install libopencv-dev libpam0g-dev libcli11-dev
    mkdir -p external && cd external
    wget -O libtorch.zip https://download.pytorch.org/libtorch/nightly/cpu/libtorch-cxx11-abi-shared-without-deps-2.2.0.dev20231031%2Bcpu.zip
    unzip libtorch.zip
    cd ..
    ```
2. Compile
    ```bash
    mkdir -p build && cd build
    cmake ..
    make
    ```

## Memory leak detection

Use `valgrind` to detect memory leaks.

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes <executable>
```