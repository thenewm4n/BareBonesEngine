# To complete
- ScenePlatformer::loadLevel()
    - Add a block to test collisions
    - Populate PlayerConfig struct -> change player speed in sMovement() according to PlayerConfig struct
    - Uncomment speed cap in sMovement().
- ScenePlatformer::spawnPlayer()
    - Are there more components to add to player?
- Implementation of sDoAction() for ScenePlatformer; including change state of player so that Animation changes.
- Bullet animation: get bullet sprite
- ScenePlatformer::sMovement(): remember that player Y speed is positive in level_1.txt, so must be negated in code
- Physics.h
- Text for SceneStartMenu - levels, controls legend at bottom
- ScenePlatformer::sRender()/sUserInput - fix colour change of pausing
    - Also, can still toggle textures etc when paused -> probably because sDoAction is still called when paused?
- Scene::setPaused() - is it necessary? Maybe, in order to stop sDoAction() when toggling textures etc while paused.
- Fix jittery camera when moving.

# Recommended order of implementation
1. Implement Scene_Play::loadLevel()
    - Once you correctly read in different type of entities, add them to EntityManager and they should automatically be drawn to screen (because rendering already done). Add the correct bounding boxes to Tile entities, and no bounding boxes to Dec entities.
2. Implement Scene_Play::spawnPlayer().
    - Read the Player configuration from the level file (into a playerConfig struct?) and spawn the player.
    - This is where the player should restart when they die.
3. Implement Scene_Play::spawnBullet().
    - Bullet should shoot in same direction player is facing when space is pressed.
    - Holding down spawn should only fire one bullet i.e. a  new bullet can only be fired after space released (when space pressed, shoot, and set CInput.canShoot to false; when space released, set CInput.canShoot to true.).
4. Implement Scene_Play::getOverlap().
    - If either entity has no bounding box, return Vec2(0, 0).
5. Implement collision checking with bullets/brick tiles such that brick is destroyed when a bullet collides with it.
    - Remember, collision occurs when overlap is non-zero in both x and y components.
    - Bullets should be destroyed upon colliding with a non-decorative tile.
    - If tile has brick animation, destroy it upon collision as well.
6. Implement collision resolution such that when player collides with non-decorative tile, player cannot enter it or overlap it (?). When player collides with tile from below, its y velocity should be set to zero so it falls back downward and doesn't hover below tile.
    - Update CState to store whether on ground on in air -> used by Animation system.
7. Implement detection of which side player collided with tile.
8. Change controls such that they are in proper l/r/jump style.
    - Note: all movement logic should be in movement system; the sDoAction system is onyl used to set CInput variables; shouldn't modify player's speed or position in sDoAction system.
9. Implement gravity such that player falls toward bottom of screen and lands on tiles with it collides with tiles from above.
    - Note that when player lands on tile from above, set vertical speed to zero so gravity doesn't continue accelerating player downward (this will happen every frame the player is on the platform, because gravity always acts).

# Improvements
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.
- Use mass in collision resolution.