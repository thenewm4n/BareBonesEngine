# To complete
1. Scene_Play::spawnBullet().
    - Clean it up and add comments.
    - Implement sLifespan().
    - Add CLifepan
2. Implement destruction of brick if player collides from below.
3. Implement collision checking with bullets/brick tiles -> brick destroyed when bullet collides with it.
    - If solid has brick animation, destroy it upon collision as well.
4. Clean up README.


# Improvements
- Fix jittery camera when moving.
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.
- Use mass in collision resolution.
- Controls legend in SceneStartMenu.