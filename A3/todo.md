# To complete
1. Isolate arrow sprites.
1. Increase size of all other textures to 64 -> so archer is smaller, hence not causing issue with brick collisions.
2. Change sMovement() logic for new player animations.
3. Increase m_gridCellSize.x & y to 64.
5. Add coin animation upon hitting question mark.
6. Implement collision checking with bullets/brick tiles -> brick destroyed when bullet collides with it.
    - If solid has brick animation, destroy it upon collision as well.
7. Clean up README (use LLM).
8. Do I need all those methods in ScenePlatformer?


# Improvements
- Fix jittery camera when moving.
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.
- Use mass in collision resolution.
- Controls legend in SceneStartMenu.