# Facepass

A FaceID login module for Linux.

## Why Facepass?

While there is an existing FaceID login module for Linux called [Howdy](https://github.com/boltgolt/howdy), it is no longer actively maintained (last release: Sep 2, 2020). Many of its dependencies are deprecated, and it lacks spoof detection capabilities. To address these issues, we—@npvinh and @thaitran24—created Facepass, a modern and secure alternative.

## Installation

### Using Debian Package

```sh
# Download the Debian package
# Replace <version> with the actual version number you want to download
wget https://github.com/TickLabVN/facepass/releases/download/<version_tag>/facepass-<version_tag>-ubuntu-22.04.deb -O facepass.deb

# Install the package
sudo dpkg -i facepass.deb

# Fix any dependency issues
sudo apt install --fix-broken
```

### Adding Your Face

After installing Facepass, add your face to the system by running:
```sh
facepass add
```

A window will appear, prompting you to look at the camera. Ensure your face is well-lit and clearly visible. Press `Esc` to capture your face and close the window. Your face data will be saved in `~/.config/facepass`.

### Enabling Face Login

To enable face ID login, follow these steps:
1. Count the number of modules above `pam_deny.so` in the `/etc/pam.d/common-auth` file:
    ```sh
    grep -n '^[^#]*pam_deny.so' /etc/pam.d/common-auth | cut -d: -f1 | xargs -I {} bash -c 'sed "1,$(({}))!d;/^#/d;/^\s*$/d" /etc/pam.d/common-auth | wc -l'
    ```
    This command will return a number, let's call it `N`.
2. In the `/etc/pam.d/gdm-password` file, locate this line:
    ```sh
    ```
3. Assume `N` = 2, edit the `/etc/pam.d/gdm-password` file (you may need `sudo` privileges), insert the following line above `@include common-auth`:
    ```sh
    auth [success=2 default=ignore] libfacepass_pam.so
    @include common-auth
    ```
4. Log out and log back in to test. Facepass will attempt to authenticate your face first. If it fails, you will be prompted to enter your password.

Explanation of PAM Configuration: the line `auth [success=2 default=ignore] libfacepass_pam.so` configures the PAM (Pluggable Authentication Module) system to handle face recognition. Here's how it works:
- If face recognition is **successful**, the system skips the next `N` modules (in this case, `N = 2`) and proceeds to the subsequent module. This allows the user to log in without entering a password.
- If face recognition **fails**, the system moves to the next module, which is `@include common-auth`. This prompts the user to enter their password.
To determine the value of `N`, we count the number of modules above `pam_deny.so` in the `/etc/pam.d/common-auth` file. This ensures that the configuration aligns with the system's authentication flow. Please visit https://docs.oracle.com/cd/E19683-01/817-0365/pam-36/index.html for more details.

### Handling Camera or Lighting Issues

If your RGB camera is not ready or the lighting conditions are poor, you can adjust the following options:

- `retries`: Sets the maximum number of authentication attempts. The default is 10.
- `retry_delay`: Specifies the delay (in milliseconds) between each retry attempt. The default is 200 milliseconds.

For example, to increase the maximum retries to 20 and set the delay between retries to 200 milliseconds, modify the line as follows:

```sh
auth [success=2 default=ignore] libfacepass_pam.so retries=20 retry_delay=200
```

## How to Contribute

We welcome contributions from the community! Here’s how you can help:

1. **Report Issues**: Found a bug or have a feature request? Open an issue on our [GitHub repository](https://github.com/TickLabVN/facepass/issues).
2. **Submit Pull Requests**: Fix bugs, improve documentation, or add new features. Check out our [contribution guidelines](https://github.com/TickLabVN/facepass/blob/main/docs/contributing.md) for more details.
3. **Test and Provide Feedback**: Help us improve by testing Facepass on different Linux distributions and hardware setups.
4. **Spread the Word**: Share Facepass with your friends and colleagues to grow our community.

## Future Work

We are actively working on the following features and improvements:

- [ ] **GUI for Face Management**: A user-friendly interface to manage multiple faces.
- [ ] **Extended Login Support**: Apply face recognition to other login methods (e.g., `sudo`, `su`). See [issue #5](https://github.com/TickLabVN/facepass/issues/5).
- [x] **Face Anti-Spoofing**: Enhance security with anti-spoofing measures. This feature will be optional for users with weaker cameras.
- [ ] **IR Camera Support**: Expand compatibility to include infrared cameras. Currently, only RGB cameras are supported.
- [ ] **Keyring Unlock**: User must enter password on the first login time to unlock applications, see https://askubuntu.com/a/238055 for more details. We will find the way to eliminate it.

Join us in shaping the future of Facepass!
