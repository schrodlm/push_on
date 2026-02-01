# Push On

A top-down, co-op roguelike with procedural generation and resource gathering.

## Features (Planned)
- Top-down shooter gameplay
- Procedural level generation
- Co-op multiplayer
- Crafting system
- Boss battles with unique drops
- Fast-paced combat inspired by Hotline Miami

## Platform Support
- **Development Platform:** Linux
- **Target Platforms:** Linux, Windows
- **Distribution:** Steam
- **Networking:** Steamworks API (Steam Networking Sockets)

## Building

### Prerequisites
- CMake 3.15+
- C++17 compiler (GCC on Linux, MSVC on Windows)
- Raylib library
- Steamworks SDK (for multiplayer features)

### Install Raylib (Linux)
```bash
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make
sudo make install
```

### Install Steamworks SDK
1. Download from [Steamworks Partner site](https://partner.steamgames.com/)
2. Extract to `external/steamworks/` (or set `STEAMWORKS_SDK_PATH`)
3. For testing, use Steam AppID 480 (Spacewar)

### Build the game (Linux)
```bash
./build.sh
# or manually:
mkdir build && cd build
cmake ..
make
./bin/push_on
```

### Build for Windows (cross-compile or native)
```bash
# Using MinGW cross-compile (from Linux)
mkdir build-windows && cd build-windows
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-w64.cmake ..
make

# Or build natively on Windows with MSVC
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

## Controls
- WASD: Movement
- Mouse: Aim
- Left Click: Shoot (TODO)
- E: Interact (TODO)

## Development Roadmap

### Phase 1: Core Mechanics
- [x] Basic movement
- [ ] Shooting mechanics
- [ ] Enemy AI
- [ ] Health/damage system
- [ ] Basic procedural generation

### Phase 2: Content
- [ ] Weapon variety
- [ ] Enemy types
- [ ] Crafting system
- [ ] Visual polish
- [ ] Boss enemies

### Phase 3: Multiplayer (Steam Integration)
- [ ] Steamworks SDK integration
- [ ] Steam Networking Sockets (P2P)
- [ ] Lobby system (Steam lobbies)
- [ ] Friend invites
- [ ] Co-op gameplay (2-4 players)
- [ ] Steam achievements
