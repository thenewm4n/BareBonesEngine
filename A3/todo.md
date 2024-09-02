# To complete
1. Implement Scene_Play::spawnBullet().
    - Bullet animation: get bullet sprite
    - Bullet should shoot in same direction player is facing when space is pressed.
    - Holding down spawn should only fire one bullet i.e. a  new bullet can only be fired after space released (when space pressed, shoot, and set CInput.canShoot to false; when space released, set CInput.canShoot to true.).
2. Implement collision checking with bullets/brick tiles such that brick is destroyed when a bullet collides with it.
    - Remember, collision occurs when overlap is non-zero in both x and y components.
    - Bullets should be destroyed upon colliding with a non-decorative tile.
    - If tile has brick animation, destroy it upon collision as well.
3. Implement detection of which side player collided with tile.
4. Clean up README.


# Improvements
- Fix jittery camera when moving.
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.
- Use mass in collision resolution.
- Controls legend in SceneStartMenu.