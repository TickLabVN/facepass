# Facepass

A FaceID login module for Linux.

## Why?

There's also a FaceID login module for Linux: [Howdy](https://github.com/boltgolt/howdy). However, Howdy is not actively maintained. Last release is on Sep 2, 2020 and many dependencies are deprecated. Howdy also cannot recognize spoofed faces. That's why I decided to create Facepass.

## Architecture

COMING SOON

## Prerequisites

Need to install the following packages:
- `libopencv-dev`
- `libpam0g-dev` (Ubuntu/Debian) or `pam-devel` (Fedora/CentOS)
- `libtorch` (PyTorch C++ API)
    ```bash
    cd include/
    wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-cxx11-abi-static-without-deps-2.2.0.dev20231031+cpu.zip
    unzip libtorch-cxx11-abi-static-without-deps-2.2.0.dev20231031+cpu.zip
    ```