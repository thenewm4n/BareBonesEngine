# Problems
- sMovement(): rebounding off walls doesn't work -> moves into wall.
- sMovement()/sUserInput(): player movement doesn't work.
- spawnEnemies(): RNG seems disfunctional...
	- Enemies spawn in same place.
	- Enemies have same fill and outline colours.
	- Enemies have same velocities
- spawnSmallEnemies(): small enemies spawn more small enemies.
- spawnBullet(): Shooting has opposite velocity.
- sLifespan(): alpha of small enemies and bullets doesn't fade.


# Order of Implementation
1. [X] Implement Vec2 class -> then test in main.cpp.
2. [X] Implement basic functionality of EntityManager class.
	- [X] Implement EntityManager::addEntity() and ::update() (don't worry about deleting dead Entities yet).
3. Implement basics of Game class.
	- [X] Construct a player Entity using spawnPlayer() function.
	- [X] Implement basic drawing of entities using the Game::sRender function.
	- [X] Construct some enemies using the spawnEnemy() function.
		- [X] Random position
		- [X] Random velocity
		- [X] Random vertices
	- [X] Construct a bullet using the spawnBullet() function.
		- [X] cTransform
		- [X] cCollision
		- [X] cLifespan
		- [X] cShape
4. [X] Implement player movement in Game::sUserInput and Game::sMovement
5. [X] Implement the EntityManager::update() function so it deletes dead enemies
6. [X] Implement the EntityManager::getEntities(tag) functionality
7. [X] Implement collisions in sCollision and entity.destroy() if it's dead
	- [X] Implement spawnSmallEnemies().
		- When (large) enemies collide with bullet or player, are destroyed, and N small enemies are spawn in its place (where N is number of vertices).
		- Each small enemy has same number of vertices and colour, and are fired off at every (360 / vertices) degrees.
8. [X] Implement rest of game's functionality including config file reading
9. Implement GUI functionality (can do this earlier to help debug).
10. [X] Implement sLifespan using RGBA.
11. [ ] Implement special weapon.

- Where to set m_text position to top left, and update m_text so it reflects m_score?