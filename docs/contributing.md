# Contributing Guidelines

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
