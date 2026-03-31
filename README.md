
# BareBones Engine

*A minimal game engine to help me understand games, from the bottom up, and make my own.*

---

### Features
- Entity-Component-System (ECS) architecture for composable functionality
- Basic physics system including gravity, collision detection and resolution
- Sprite-based animation system
- Remappable control scheme
- Simple level configuration
- Debug rendering options (toggle textures, bounding boxes, grid)
- Scene management

### Prerequisites
- C++17 (or later) compatible compiler
- CMake 4.0 or later
- OpenGL development headers
- Operating System: Windows, macOS, or Linux (Ubuntu 18.04+)
- An active internet connection (to automatically download SFML during CMake configuration).
Note: Exact version requirements may vary. Please refer to the CMakeLists.txt file for the most up-to-date dependency information.

### Setup Instructions
1. Clone the Repository:
    ```
    git clone https://github.com/thenewm4n/BareBonesEngine.git
    cd BareBonesEngine
    ```

2. Build the Project:
    ```
    cmake -B build
    cmake --build build
    ```

3. Run the Game:

    **On Linux / macOS:**
    ```bash
    ./bin/BareBones
    ```

    **On Windows:**
    ```powershell
    .\bin\BareBones.exe
    ```
    *Note: might require .\bin\Debug\BareBones.exe or .\bin\Release\BareBones.exe, depending on your CMake generator.*

### Usage
#### Config File
Define window dimensions and framerate:
```
Window W H F
    W   Width       int
    H   Height      int
    F   Framerate   int
```

#### Assets File
Applying a texture to an entity can be done by adding an Animation component with a frame count and frame duration of 1, and toRepeat parameter set to true.
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
> Note: A texture used in an animation must be defined before the animation.

#### Level File
Specify initial conditions of level including environment and player entity properties:
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

### Controls
#### Menu scene
- W: Up
- S: Down
- Enter: Select
- Tilda (~): Open Level Editor
- ESC: Exit Game

#### Platformer scene
- W: Jump
- A: Move left
- D: Move right
- Space: Shoot arrow
- P: Pause game
- T: Toggle drawing textures
- B: Toggle drawing bounding boxes
- G: Toggle drawing grid
- ESC: Return to main menu

#### All scenes
- L: Screenshot

### Acknowledgments
- SFML: https://www.sfml-dev.org/
- ImGui: https://github.com/ocornut/imgui
- Archer animations: https://chierit.itch.io/elementals-leaf-ranger
- Explosion animation: https://xthekendrick.itch.io/2d-explosions-pixel-art-pack-1
- Mario sprites: https://www.spriters-resource.com/nes/supermariobros/

### License
This project is licensed under the MIT License - see the LICENSE file for details.

### Contact
Feel free to reach out with any questions or suggestions! You can contact me at: bobby.newman@outlook.com