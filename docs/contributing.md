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

## Development

**WARNING**: Before you start, please make sure:
- Prepare a bootable USB drive with a Linux distribution (e.g., Ubuntu), or make sure you have recovery mode. Because of editing the `/etc/pam.d/common-auth` file, you may lost access to your system if you make a mistake. You can use the USB drive to boot into a live environment, mount your hard drive, modify the `common-auth` file, and then reboot into your system.
- Grant yourself edit permission to the `/etc/pam.d/common-auth` file.
   ```bash
   sudo chown -R $(whoami) /etc/pam.d/common-auth
   sudo chmod +w /etc/pam.d/common-auth
   ```
After compiling, follow these steps:
1. Run `./build/cli/facepass add` to open a window to add your face. Press `Esc` to save and exit.
2. Copy the `libfacepass_pam.so` to `/lib/security/`
   ```bash
   sudo cp build/auth/libfacepass_pam.so /lib/security/
   ```
3. Add the line `auth sufficient libfacepass_pam.so` to the top of file `/etc/pam.d/common-auth`. You may need `sudo` permission to edit this file.
4. Now type `sudo -i` to test if the Facepass PAM module works. If it does, you should be able to login with your face.