# To complete
1. Should I have specific getters in ConfigManager e.g. getResolution?
2. Replace config with ConfigManager.
3. Clean up README (use LLM).
4. Do I need all those methods in ScenePlatformer?
5. Is there point in WEAPON in PlayerConfig in ScenePlatformer?
6. Change size/resolution of window according to windows window.


# Improvements
- Fix jittery camera when moving.
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.
- Use mass in collision resolution.
- Controls legend in SceneStartMenu.