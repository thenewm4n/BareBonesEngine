# To complete
1. Replace config with ConfigManager.
    - Including title of window (GameEngine::init())
2. Should I have specific getters in ConfigManager e.g. getResolution?
3. Clean up README.
4. Do I need all those methods in ScenePlatformer?
5. Is there point in WEAPON in PlayerConfig in ScenePlatformer?
6. Change size/resolution of window according to windows window.


# Improvements
- Use profiler to optimise screenshot
- Fix jittery camera when moving -> could be due to frame rate dependency in the camera movement logic -> ensure camera updates are decoupled from frame rate by using delta time for smooth transitions.
- SceneStartMenu: make initialisation of title, menu strings & level paths dynamic according to e.g. config file, levels file.
- Decouple logic from framerate using delta time.
- Use mass in collision resolution.
- Controls legend in SceneStartMenu.