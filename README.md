# Autonomous Agent Project

![Game Preview](readMePic.png)

## Overview
This is a bus-themed game built with [raylib](https://www.raylib.com/). Control a fleet of buses as they race to collect targets, refuel, and compete for the highest score. The game features multiple stages, fuel management, and dynamic bus behavior.

## How to Build & Run

### Prerequisites
- [raylib](https://www.raylib.com/) (automatically fetched if not found)
- [CMake](https://cmake.org/) (version 3.17 or higher)
- C++ compiler (C++11 or higher)

### Build Steps
1. **Clone this repository**
2. **Open a terminal in the project root**
3. **Run the following commands:**
   ```sh
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```
4. **Run the game:**
   - On most systems: `./raylib_quickstart_main_forreal`
   - On Windows: `raylib_quickstart_main_forreal.exe`

### Resource Directory
The game uses a `resources` folder for images and assets. The executable will search for this folder in several common locations (working directory, app directory, up to 3 levels above). Make sure the `resources` folder is present in the project root or next to the executable.

## Gameplay
- **Objective:** Control a fleet of buses to collect as many blue targets as possible before running out of fuel.
- **Buses:** Each bus moves toward a target. When a bus collects a target, it refuels and scores a point. If a bus runs out of fuel, it starts wandering randomly.
- **Stages:** Every 20 seconds, the stage increases. Buses are ranked by score, and the lowest-scoring bus is removed. Top buses get a fuel capacity bonus (up to a max).
- **Power-ups:** Buses occasionally get a speed boost for a short duration.
- **Game Over:** The game continues until only one bus remains.

## Controls
- The game is fully automated; buses are AI-controlled. Just watch the action unfold!
- Close the window to exit.

## Credits
- Built with [raylib](https://www.raylib.com/)
- Resource directory helper by Jeffery Myers (MIT License)

---

Enjoy the game!
=======

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
vcpkg install raylib
>>>>>>> d3e2938e8ff75ef9292c6cc311917ba81a67c834
