# Facepass

A FaceID login module for Linux.

## Why?

There's also a FaceID login module for Linux: [Howdy](https://github.com/boltgolt/howdy). However, Howdy is not actively maintained. Last release is on Sep 2, 2020 and many dependencies are deprecated. Howdy also cannot recognize spoofed faces. That's why I and @thaitran24 decided to create Facepass.


## Installation

### Using Debian Package

```sh
# Download the Debian package
# Replace <version> with the actual version number you want to download
wget http://deb-pkg.ticklab.site/pkool/main/f/facepass/facepass-<version>-Linux.deb -O facepass.deb

# Install the package
sudo dpkg -i facepass.deb

# Fix any dependency issues
sudo apt install --fix-broken
```

### Using APT Repository

```sh
# Add the repository to your sources list
echo "deb [trusted=yes] http://deb-pkg.ticklab.site/ stable main" | sudo tee /etc/apt/sources.list.d/facepass.list

# Update the package list
sudo apt update

# Install Facepass
sudo apt install facepass
```
### Adding your face

After installing Facepass, you need to add your face to your system. Run the following command:
```sh
facepass add
```

A window will pop up, prompting you to look at the camera. Make sure your face is well-lit and clearly visible. Press `Esc` to snapshot your face and close the window. Your face will be save in `~/.config/facepass`.

### Enabling face login

To enable face login, you need to edit the `/etc/pam.d/gdm-password` file. Open the file with your favorite text editor (you may need sudo permission), find this line:

```sh
@include common-auth
```

Add the following line above `@include common-auth`:
```sh
auth	[success=2 default=ignore]	libfacepass_pam.so
@include common-auth
```

Now try to logout and login again. Your face should be tried first. If it fails, you will be prompted to enter your password.

### Future work

- [ ] Add a GUI to manage multiple faces
- [ ] Apply face recognition to other login methods (e.g. `sudo`, `su`, etc.). Currently, it only works for `login`. See the issue https://github.com/TickLabVN/facepass/issues/5 for more information.
- [ ] Apply face anti-spoofing. This feature may not work on weak cameras, so we will let user enable this option or not in the GUI.
- [ ] Support IR camera. Currently, we only support RGB camera.