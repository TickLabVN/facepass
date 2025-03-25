# Contributing Guidelines

## How to Compile

### Step 1: Install Dependencies
Run the following commands to install the required dependencies and set up the environment:
```bash
sudo apt install libopencv-dev libpam0g-dev libcli11-dev
mkdir -p external && cd external
wget -O libtorch.zip https://download.pytorch.org/libtorch/nightly/cpu/libtorch-cxx11-abi-shared-without-deps-2.2.0.dev20231031%2Bcpu.zip
unzip libtorch.zip
cd ..
```

### Step 2: Compile the Project
Execute the following commands to compile the project:
```bash
mkdir -p build && cd build
cmake ..
make
```

## Development Guidelines

### Important Warnings
Before proceeding, ensure the following:
- **System Recovery Preparation**: Prepare a bootable USB drive with a Linux distribution (e.g., Ubuntu) or ensure you have access to recovery mode. Editing the `/etc/pam.d/common-auth` file incorrectly may lock you out of your system. Use the USB drive to boot into a live environment, mount your hard drive, and fix the `common-auth` file if needed.
- **File Permissions**: Grant yourself edit permissions for the `/etc/pam.d/common-auth` file:
   ```bash
   sudo chown -R $(whoami) /etc/pam.d/common-auth
   sudo chmod +w /etc/pam.d/common-auth
   ```

### Post-Compilation Steps
After successfully compiling the project, follow these steps:

1. **Add Your Face**:
   Run the following command to open a window for adding your face. Press `Esc` to save and exit:
   ```bash
   ./build/cli/facepass add
   ```

2. **Copy the PAM Module**:
   Copy the `libfacepass_pam.so` file to the `/lib/security/` directory:
   ```bash
   sudo cp build/auth/libfacepass_pam.so /lib/security/
   ```

3. **Modify PAM Configuration**:
   Add the following line to the top of the `/etc/pam.d/common-auth` file. You may need `sudo` permissions to edit this file:
   ```
   auth sufficient libfacepass_pam.so
   ```

4. **Test the PAM Module**:
   Test if the Facepass PAM module works by typing:
   ```bash
   sudo -i
   ```
   If successful, you should be able to log in using facial recognition.
