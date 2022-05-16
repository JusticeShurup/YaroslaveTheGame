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
#include "Server.h"

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

	Player* getPlayer();
	void addPlayer(Entity* player);
	Camera* getCamera();
	Server* getServer();


	void run();
	void update();
	void render();

	void removeEnemy(Entity* enemy);

private:
	sf::Thread server_thr;
	Server server;

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

	Player* main_player;
	std::map<std::string, Entity*> players;
	
	sf::RectangleShape background;
	GUI* gui;

	std::vector<Enemy*> enemies;

	sf::Text gameover_text;

};
#endif
