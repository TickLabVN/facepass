# Facepass

A FaceID login module for Linux.

## Why?

There's also a FaceID login module for Linux: [Howdy](https://github.com/boltgolt/howdy). However, Howdy is not actively maintained. Last release is on Sep 2, 2020 and many dependencies are deprecated. Howdy also cannot recognize spoofed faces. That's why I and @thaitran24 decided to create Facepass.

## Installation

### Using Debian Package

```sh
# Download the Debian package
wget http://deb-pkg.ticklab.site/pool/main/f/facepass/facepass-0.1.0-Linux.deb -O facepass.deb

# Install the package
sudo dpkg -i facepass.deb

# Fix any dependency issues
sudo apt-get install -f
```

### Using APT Repository

```sh
# Add the repository to your sources list
echo "deb [trusted=yes] http://deb-pkg.ticklab.site/ stable main" | sudo tee /etc/apt/sources.list.d/facepass.list

# Update the package list
sudo apt-get update

# Install Facepass
sudo apt-get install facepass
```