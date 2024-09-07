# To complete
1. Change bounding box to be set to bottom of sprite - not centre.
1. Implement collision checking with arrows/brick tiles -> brick destroyed when bullet collides with it.
    - Refactor code in handlePlayerCollisions for Brick and QMark blocks.
2. Decrease size of explosion animation frames to < 64x64.
3. Clean up README (use LLM).
4. Do I need all those methods in ScenePlatformer?
5. Clean up shooting animation.
6. Is there point in WEAPON in PlayerConfig in ScenePlatformer?


# Improvements
- Fix jittery camera when moving.
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.
- Use mass in collision resolution.
- Controls legend in SceneStartMenu.