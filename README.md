
# BareBones Engine

## Overview
Welcome to my C++ game engine, powered by SFML for graphics and sound. This engine is designed to help understand the ECS (Entity-Component-System) architecture through practical implementation.

## Features
- Entity-Component-System (ECS) architecture for efficient game object management
- SFML-powered graphics and sound system
- Customizable window settings (resolution, fullscreen, VSync, max FPS)
- Easy-to-define assets (textures, animations, fonts)
- Simple level configuration system
- Remappable control scheme
- Basic physics including collision detection and resolution
- Debug rendering options (toggle textures, bounding boxes, grid)
- Scene management (Start Menu, Platformer gameplay)

## Prerequisites
- C++17 (or later) compatible compiler
- SFML 2.5.1 or later
- CMake 3.10 or later
- Operating System: Windows, macOS, or Linux (Ubuntu 18.04+)
Note: Exact version requirements may vary. Please refer to the CMakeLists.txt file for the most up-to-date dependency information.

## Setup Instructions
1. Install SFML:
   - On macOS with Homebrew: `brew install sfml`
   - On Linux: Use your package manager or see the [SFML download page](https://www.sfml-dev.org/download.php)
   - On Windows: Download from the [SFML website](https://www.sfml-dev.org/download.php) and extract to a known location

2. Clone the Repository:
   ```
   git clone https://github.com/your-username/BareBones.git
   cd BareBones
   ```

3. Build the Project:
   ```
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

4. Run the Game:
   ```
   ./BareBones
   ```

## Usage
### Config File
Define window dimensions and framerate:
```
Window W H F
    W   Width       int
    H   Height      int
    F   Framerate   int
```

### Assets File
> Note: A texture used in an animation must be defined before the animation.
List textures, animations, and fonts:
```
Texture N P
    N   Name        std::string
    P   FilePath    std::string

Animation N T F D
    N   AnimationName   std::string
    T   TextureName     std::string
    F   FrameCount      int
    D   FrameDuration   int

Font N P
    N   FontName    std::string
    P   FontPath    std::string
```


### Level File
Specify game levels and player properties:
```
Player GX GY BW BH SX SY SM G W
    GX,GY   GridPositionX,Y         float,float
    BW,BH   BoundingBoxW,H          float,float
    SX      HorizontalSpeed         float
    SY      JumpSpeed               float
    SM      MaxSpeed                float
    G       Gravity                 float
    W       WeaponName              std::string

Solid N GX GY
    N   AnimationName   std::string
    GX  GridPositionX   float
    GY  GridPositionY   float

Prop N GX GY
    N   AnimationName   std::string
    GX  GridPositionX   float
    GY  GridPositionY   float
```
> Note: GX and GY positions in the level specification file are in 'grid' coordinates. Each 'grid' cell measures 64 by 64 pixels. Entities are positioned so that the bottom-left corner of their texture aligns with the bottom-left corner of the specified grid coordinate.
> Note: Solid entities have bodies, i.e., bounding boxes, whereas props don't.

## Controls
### Menu scene
- W: Up
- S: Down
- ESC: Exit game

### Platformer scene
- W: Jump
- A: Move left
- D: Move right
- Space: Shoot arrow
- P: Pause game
- T: Toggle drawing textures
- B: Toggle drawing bounding boxes
- G: Toggle drawing grid
- ESC: Return to main menu

### All scenes
- L: Screenshot

## Acknowledgments
- SFML: https://www.sfml-dev.org/
- ImGui: https://github.com/ocornut/imgui
- Archer animations: https://chierit.itch.io/elementals-leaf-ranger
- Explosion animation: https://xthekendrick.itch.io/2d-explosions-pixel-art-pack-1
- Mario sprites: https://www.spriters-resource.com/nes/supermariobros/

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Contact
Feel free to reach out with any questions or suggestions! You can contact me at: bobby.newman@outlook.com