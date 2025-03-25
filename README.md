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

To enable face login, edit the `/etc/pam.d/gdm-password` file. Open it with your preferred text editor (sudo permissions required), locate this line:

```sh
@include common-auth
```

Insert the following line above `@include common-auth`:
```sh
auth	[success=2 default=ignore]	libfacepass_pam.so
@include common-auth
```

Log out and log back in to test. Facepass will attempt to authenticate your face first. If it fails, you will be prompted to enter your password.

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
- [ ] **Face Anti-Spoofing**: Enhance security with anti-spoofing measures. This feature will be optional for users with weaker cameras.
- [ ] **IR Camera Support**: Expand compatibility to include infrared cameras. Currently, only RGB cameras are supported.

Join us in shaping the future of Facepass!