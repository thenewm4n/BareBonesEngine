
# BareBones Engine

## Overview
Welcome to my C++ game engine, powered by SFML for graphics and sound. This engine is designed to help understand the ECS (Entity-Component-System) architecture through practical implementation.

## Features
- Customizable window settings
- Easy-to-define assets (textures, animations, fonts)
- Simple level configuration
- Remappable control scheme

## Prerequisites
- C++17 or later
- SFML 2.5.1 or later

## Setup Instructions
Clone the Repository:
```
git clone https://github.com/your-username/your-repo.git
cd your-repo
```

Build the Project:
```
mkdir build
cd build
cmake ..
make
```

Run the Game:
```
./game
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
    W       WeaponAnim              std::string

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

## Controls for Platformer scene
W: Jump
A: Move left
D: Move right
P: Pause game
T: Toggle drawing textures
C: Toggle drawing bounding boxes
G: Toggle drawing grid
ESC: Return to main menu

## License
This project is licensed under the MIT License - see the LICENSE file for details.