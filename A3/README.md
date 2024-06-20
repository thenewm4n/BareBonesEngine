# Config File Specification
Window W H F
    W   Width       int
    H   Height      int
    F   Framerate   int

# Assets File Specification
*Textures*
**Texture used in an animation must be defined before Animation.**
Texture N P
    N   Name        std::string
    P   FilePath    std::string


*Animations*
Animation N T F S
    N   AnimationName   std::string
    T   TextureName     std::string
    F   FrameCount      int (no. frames in Animation)
    S   AnimationSpeed  int (no. game frames between anim frames AKA frameDuration)

*Fonts*
Font N P
    N   FontName    std::string
    P   FontPath    std::string

# Config File Specification
*Window*
Window W H F M
    WindowWidth W int
    WindowHeight H int
    FrameRate F int
    Toggle Full Screen Mode M bool (0/1)


# Level File Specification
- Game levels will be specified by a Level file, which will contain a list of entity specifications, one per line. It will also contain a single line which specifies properties of the player in that level. In this way, you can define an entire level in the data file, rather than in programming code. The syntax of the lines of the Level file are as follows.

*IMPORTANT NOTE*
All (GX, GY) positions given in the level specification file are given in 'grid' coordinates. The 'grid' cells are of size 64 by 64 pixels, and the entity should be positioned such that the bottom left corner of its texture is aligned with the bottom left corner of the given grid coordinate. The grid starts at (0,0) in the bottom-left of the screen, and can be seen by pressing the 'G' key while the game is running.

*Tile*
Tile N GX GY
    N   AnimationName   std::string
    GX  GridXPos        float
    GY  GridYPos        float

*Decoration*
Dec N X Y
    AnimationName   N   std::string
    XPosition       X   float
    YPosition       Y   float

*Player*
Player GX GY CW CH SX SY SM GY B
    GridPosX,Y          GX,GY   float,float
    BoundingBoxW,H      CW,CH   float,float
    HorizontalSpeed     SX      float
    JumpSpeed           SY      float
    MaxSpeed            SM      float
    Gravity             G       float
    BulletAnim          B       std::string

### Keys: (All are remappable)
- W: Jump
- A: Move left
- D: Move right
- P: Pause game
- T: Toggles drawing textures
- C: Toggles drawing bounding boxes of entities
- G: Toggles drawing of the grid
- ESC: Toggle menu



# Assets
- Entities in the game will be rendered using various Textures and Animations which we will be calling Assets (along with Fonts).
- Assets are loaded once at the beginning of the program and stored in the Assets class, which is stored by the GameEngine class.
- All Assets are defined in assets.txt, with the syntax defined below.

*Note*:
- All entity positions denote the center of their rectangular sprite.
- It also denotes the center of the bounding box, if it has one
- This is set via sprite.setOrigin() in the Animation class constructor.

# Player
- The player Entity in the game is represented by Megaman, which has several different Animations: Stand, Run, and Air. You must determine which state the player is currently in and assign the correct Animation.
- The player moves with the following controls:
    - Left: A key, Right: D key, Jump: W Key, Shoot: Space Key
- The player can move left, move right, or shoot at any time during the game.
- This means the player can move left/right while in the air.
- The player can only jump if it is currently standing on a tile.
- If the jump key is held, the player should not continuously jump, but instead it should only jump once per button press. If the player lets go of the jump key mid-jump, it should start falling back down immediately.
- If the player moves left/right, the player's sprite will face in that direction until the other direction has been pressed.
- Bullets shot by the player travel in the direction the player is facing.
- The player collides with 'Tile' entities in the level (see level syntax) and cannot move through them. The player land on a Tile entity and stand in place if it falls on it from above.
- The player does not collide with 'Dec' (decoration) entities in the level.
- If the player falls below the bottom of the screen, they respawn at the start.
- The player should have a Gravity component which constantly accelerates it downward on the screen until it collides with a tile.
- The player has a maximum speed specified in the Level file (see below) which it should not exceed in either x or y direction.
- The player will be given a BoundingBox of a size specified in the level file.
- The player's sprite and bounding box are centered on the player's.

# Animations
- Animations are implemented by storing multiple frames inside a texture.
- The Animation class handles frame advancement based on animation speed.
- You need to implement Animation:: update() to properly progress animations.
- You need to implement Animation:: hasEnded () which returns true if an animation has finished its last frame, false otherwise.
- Animations can be repeating (loop forever) or non-repeating (play once).
- Any entity with a non-repeating animation should be destroyed once its Animation's hasEnded() returns true (has finished one cycle).

# Decoration Entites
- Decoration entities ('Dec' in a level file) are simply drawn to the screen, and do not interact with any other entities in the game in any way I
- Decorations can be given any Animation in the game, but intuitively they should be reserved for things like clouds, bushes,

# Tiles
- Tiles are Entities that define the level geometry and interact with players.
- Tiles can be given any Animation that is defined in the Assets file.
- Tiles will be given a BoundingBox equal to the size of the animation tile-›getComponent‹CAnimation>().animation.getsize().
- The current animation displayed for a tile can be retrieved with: tile-›getComponent‹CAnimation>().animation.getName().
- Tiles have different behavior depending on which Animation they are given:
    *Brick Tiles*:
    - Brick tiles are given the 'Brick' Animation.
    - When a brick tile collides with a bullet, or is hit by a player from below:
        - Its animation should change to 'Explosion' (non-repeat).
        - Non-repeating animation entities are destroyed when hasEnded() is true.
        - Its BoundingBox component should be removed.
    *Question Tiles*:
    - Question tiles are given the 'Question' Animation when created.
    - When a Question tile is hit by a player from below, 2 things happen:
        - Its Animation changes to the darker 'Question?' animation.
        - A temporary lifespan entity with the 'Coin' animation should appear for 30 frames, 64 pixels above the location of the Question.

# Misc
- The 'p' key should pause the game.
- Pressing the 'T' key toggles drawing textures.
- Pressing the 'C' key toggles drawing bounding boxes of entities.
- Pressing the 'G' key toggles drawing of the grid.
- The 'ESC' key should go 'back' to the Main Menu, or quit if on the Main.