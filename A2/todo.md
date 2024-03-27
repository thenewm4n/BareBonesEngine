# Order of Implementation
1. Implement Vec2 class -> then test in main.cpp.
2. Implement basic functionality of EntityManager class.
	- Implement EntityManager::addEntity() and ::update() (don't worry about deleting dead Entities yet).
3. Implement basics of Game class.
	- Construct a player Entity using spawnPlayer() function.
	- Implement basic drawing of entities using the Game::sRender function.
	- Construct some enemies using the spawnEnemy() function.
	- Construct a bullet using the spawnBullet() function.
