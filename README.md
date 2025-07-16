# Autonomous Agent Project

![Autonomous Agent](./resources/your-image.png)

## Overview

This project simulates autonomous buses navigating a 2D environment to collect fuel targets. Each bus moves towards fuel pickups to avoid running out of fuel, and when fuel is depleted, buses enter a wandering state until they refuel. The simulation is built using [Raylib](https://www.raylib.com/) for rendering and input handling.

## Features

- Multiple autonomous bus agents with fuel management.
- Intelligent targeting system for fuel pickups.
- Wandering behavior when buses run out of fuel.
- Speed boost power-up mechanic.
- Scoring system based on pickups collected.
- Dynamic stage progression with target resets and bus upgrades.

## Controls

- The simulation runs automatically once started.
- Observe bus behaviors as they collect fuel and avoid running out.

## Dependencies

- C++11 or later compiler.
- [Raylib](https://www.raylib.com/) — simple and easy-to-use library to enjoy videogames programming.

## How to Install Raylib
## macOS
Install Raylib using Homebrew:
brew install raylib
Linux (Ubuntu/Debian)

### Option 1: Install from Package Manager (if available)
sudo apt-get update
sudo apt-get install libraylib-dev

### Option 2: Build from Source
sudo apt-get update
sudo apt-get install build-essential git cmake libglfw3-dev libopenal-dev libpthread-stubs0-dev libx11-dev libxcursor-dev libxi-dev libxrandr-dev libxinerama-dev libudev-dev libasound2-dev libpulse-dev

git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake ..
make
sudo make install
### On Windows

1. Download the Raylib installer or precompiled binaries from the [Raylib releases page](https://github.com/raysan5/raylib/releases).
2. Follow the instructions included for setting up the library with your IDE or build system.

Alternatively, install via **vcpkg**:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
vcpkg install raylib
