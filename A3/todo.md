# To complete
- GameEngine::sUserInput, Line 91 - check that resizing correctly changes the view (it seems to change the resolution of the view).
- SceneStartMenu constructor - check the view is set correctly -> OR JUST GET RID OF VIEW AND USE RESOLUTION VALUES (in sRender())
- - Bullet animation: get bullet sprite
- SceneStartMenu::sDoAction() - are W & S incrementing/decrementing the index?
    - Escape button works.   
- SceneStartMenu::sRender() - Set centre of text correctly (not top left corner)
- ScenePlatformer::sRender() - fix grid (doesn't show text and is half cell length off in x & y directions)
- ScenePlatformer::loadLevel()
- Implementation of sDoAction() for ScenePlatformer; including change state of player so that Animation changes.
- ScenePlatformer::sAnimation() - check player animations are correct, and all other animations (explosion, QMark etc) will work correctly
- Rest of ScenePlatformer
    - sMovement(): remember that player Y speed is positive in level_1.txt, so must be negated in code
- Physics.h
- Text for SceneStartMenu - levels, controls legend at bottom
- Ensure grid cells match size of sprites (16 pixels in length and height?)

# Recommended order of implementation
1. Rendering system already done (maybe...) -> T (textures), C (bounding boxes) and G (grid) can be used to debug
2. Can implement Animation::update() and Animation::hasEnded() at any time -> only effects animation, not gameplay
3. Implement Scene_Play::loadLevel()
    - Once you correctly read in different type of entities, add them to EntityManager and they should automatically be drawn to screen (because rendering already done). Add the correct bounding boxes to Tile entities, and no bounding boxes to Dec entities.


    - Register keys in Scene_Play::init() at this point?
    - As part of this step, implement Scene_Play::gridToMidPixel(), which takes in as parameters a grid x,y position and an Entity, and returns the Vec2 position of the center of that entity. You must use the Animation size of the Entity to determine where its center point should be (this is the only role of the Entity in this function). Keep in mind this means the Entity must already have a CAnimation.
4. Implement Scene_Play::spawnPlayer().
    - Read the Player configuration from the level file (into a playerConfig struct?) and spawn the player.
    - This is where the player should restart when they die.
5. Implement some basic WASD u/l/d/r movement for the player entity -> will help test collisions.
    - registerAction() to register actions for the scene in Scene_Play::init().
    - Scene_Play::sDoAction() to perform action.
6. Implement Scene_Play::spawnBullet().
    - Bullet should shoot in same direction player is facing when space is pressed.
    - Holding down spawn should only fire one bullet i.e. a  new bullet can only be fired after space released (when space pressed, shoot, and set CInput.canShoot to false; when space released, set CInput.canShoot to true.).
7. Implement Scene_Play::getOverlap().
    - If either entity has no bounding box, return Vec2(0, 0).
8. Implement collision checking with bullets/brick tiles such that brick is destroyed when a bullet collides with it.
    - Remember, collision occurs when overlap is non-zero in both x and y components.
    - Bullets should be destroyed upon colliding with a non-decorative tile.
    - If tile has brick animation, destroy it upon collision as well.
9. Implement collision resolution such that when player collides with non-decorative tile, player cannot enter it or overlap it (?). When player collides with tile from below, its y velocity should be set to zero so it falls back downward and doesn't hover below tile.
    - Update CState to store whether on ground on in air -> used by Animation system.
10. Implement detection of which side player collided with tile.
11. Change controls such that they are in proper l/r/jump style.
    - Note: all movement logic should be in movement system; the sDoAction system is onyl used to set CInput variables; shouldn't modify player's speed or position in sDoAction system.
12. Implement gravity such that player falls toward bottom of screen and lands on tiles with it collides with tiles from above.
    - Note that when player lands on tile from above, set vertical speed to zero so gravity doesn't continue accelerating player downward (this will happen every frame the player is on the platform, because gravity always acts).

# To do
- Animation.cpp
    - update()
- Scene_Play
    - Register gameplay actions
    - Center pixel
    - loadLevel
    - in sMovement? - when you land, reset y velocity to zero (or should this be, if on ground, set y velocity to 0?)
        - Maybe something like: if collision from top, resolve collision and set velocity to 0
- Test Vec2<T>.


# Improvements
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.