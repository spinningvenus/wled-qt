# WLED-Qt
WLED-Qt is a lightweight cross-platform desktop app written in C++ and Qt.

# Install From Release
```
Coming soon
```

# Install From Source
### Install build dependencies

#### Ubuntu
```
sudo apt-get update && sudo apt-get install build-essential cmake qt6-base-dev qt6-tools-dev
```

If you run into a build errors you may also need the following:

```
sudo apt-get install libgl1-mesa-dev libxkbcommon-dev
```

#### Arch

```
sudo pacman -Syy base-devel cmake qt6-base qt6-tools
```

If you run into a build errors you may also need the following:

```
sudo pacman -S mesa
sudo pacman -S qt6-wayland # For Wayland support
```
### Build and Install
```
cd wled-qt
mkdir build && cd build
cmake .. && make
mv ./wled-qt ~/.local/bin/
```

# Usage
Run `wled-qt` from the command line to launch the GUI. A config file is automatically created at `~/.config/wled-qt/wled.conf`


# Features
Currently toggling the led state, adjusting the brightness, and selecting a preset are implemented.

#### Future plans include:
- Optional tray mode
- Support for more than 1 device
- UI Rework
- Distro Packages