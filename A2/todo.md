# Order of Implementation
1. [X] Implement Vec2 class -> then test in main.cpp.
2. [X] Implement basic functionality of EntityManager class.
	- [X] Implement EntityManager::addEntity() and ::update() (don't worry about deleting dead Entities yet).
3. Implement basics of Game class.
	- Construct a player Entity using spawnPlayer() function.
	- Implement basic drawing of entities using the Game::sRender function.
	- Construct some enemies using the spawnEnemy() function.
	- Construct a bullet using the spawnBullet() function.
4. Implement player movement in Game::sUserInput and Game::sMovement
5. Implement the EntityManager::update() function so it deletes dead enemies
6. Implement the EntityManager::getEntities(tag) functionality
7. Implement collisions in sCollision and entity.destroy() if it's dead
8. Implement rest of game's functionality including config file reading
9. Implement GUI functionality (can do this earlier to help debug).
