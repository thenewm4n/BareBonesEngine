# To complete
1. Change sMovement() logic for new player animations.
2. Speed up shooting animation.
3. Fix direction of shooting.
5. Add coin animation upon hitting question mark.
6. Implement collision checking with bullets/brick tiles -> brick destroyed when bullet collides with it.
    - If solid has brick animation, destroy it upon collision as well.
7. Clean up README (use LLM).
8. Do I need all those methods in ScenePlatformer?
9. Clean up animations.
10. Is there point in WEAPON in PlayerConfig in ScenePlatformer?
11. Fix jumping off arrows.


# Improvements
- Fix jittery camera when moving.
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.
- Use mass in collision resolution.
- Controls legend in SceneStartMenu.