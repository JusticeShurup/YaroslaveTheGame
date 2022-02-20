#ifndef _GAME_H_
#define _GAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "Map/Map.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"

class Game
{
public:
	Game();

	void run();
	void update();
	void render();

	void removeEnemy(Entity* enemy);

private:
	sf::RenderWindow* window;
	sf::Event event;
	Camera* camera;
	sf::ContextSettings settings;
	Map* map;

	sf::Clock clock;
	float delta_time;

	bool is_running;
	bool is_paused;
	bool is_active;

	Player* player;
	std::vector<Enemy*> enemies;
};
#endif
