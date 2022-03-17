#ifndef _GAME_H_
#define _GAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "Map/Map.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "Menu/MainMenu.h"

class Game
{
public:
	Game();
	~Game();

	void setIsPaused(bool flag);
	bool isPaused();

	void restartGame();

	void setMenu(Menu* menu);

	void setIsRunning(bool flag);

	void run();
	void update();
	void render();

	void removeEnemy(Entity* enemy);

private:
	sf::RenderWindow* window;
	sf::Event event;
	Menu* menu;
	Camera* camera;
	sf::ContextSettings settings;
	Map* map;

	sf::Clock clock;
	float delta_time;
	
	float gameover_timer;
	bool is_gameover;


	bool is_running;
	bool is_paused;
	bool is_active;

	Player* player;
	std::vector<Enemy*> enemies;

	sf::Text gameover_text;

};
#endif
