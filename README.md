# Specification

## Config File
Window W H F
    W   Width       int
    H   Height      int
    F   Framerate   int


## Assets File
*Textures*
Texture N P
    N   Name        std::string
    P   FilePath    std::string

*Animations*
Animation N T F D
    N   AnimationName   std::string
    T   TextureName     std::string
    F   FrameCount      int             (no. frames in Animation)
    D   FrameDuration   int             (no. game frames between animation frames; inverse of speed)

*Fonts*
Font N P
    N   FontName    std::string
    P   FontPath    std::string

*NOTE*
A texture used in an animation must be defined before the animation.


# Level File
Game levels are defined in a Level file, which includes a list of entity specifications, each on a separate line.
It also contains a single line specifying the player's properties for that level.

*Solid*
Solid N GX GY
    N   AnimationName   std::string
    GX  GridPositionX   float
    GY  GridPositionY   float

*Prop*
Prop N GX GY
    N   AnimationName   std::string
    GX  GridPositionX   float
    GY  GridPositionY   float

*Player*
Player GX GY BW BH SX SY SM G W
    GX,GY   GridPositionX,Y          float,float
    BW,BH   BoundingBoxW,H      float,float
    SX      HorizontalSpeed     float
    SY      JumpSpeed           float
    SM      MaxSpeed            float
    G       Gravity             float
    W       WeaponAnim          std::string

*NOTE*
The GX and GY positions in the level specification file are in 'grid' coordinates. 
Each 'grid' cell measures 64 by 64 pixels.
Entities are positioned so that the bottom-left corner of their texture aligns with the bottom-left corner of the specified grid coordinate.


### Keys: (All are remappable)
- W: Jump
- A: Move left
- D: Move right
- P: Pause game
- T: Toggles drawing textures
- C: Toggles drawing bounding boxes of entities
- G: Toggles drawing of the grid
- ESC: Toggle menu