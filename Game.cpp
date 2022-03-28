#include "Game.h"
#include "TextureContainer/TextureContainer.h"
#include "Player.h"
#include "Sounds/SoundContainer.h"
#include <typeinfo>
#include <thread>
#include <iostream>

Game::Game() {
	settings.antialiasingLevel = 100;
	window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "YaroslaveTheGame", sf::Style::Default, settings);

	event = sf::Event();
	TextureContainer::getInstance(); 
	SoundContainer::getInstance();
	map = new Map("Map\\Map.txt", "Map\\GameObjectsMap.txt");
	menu = new MainMenu(window, this);
	is_active = true;

	player = new Player(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "NES_Slave", 100, 100, 0.15, 20, "Yaroslave");
	player->setNewHitboxPosition(sf::Vector2f(4, 8));
	player->setGame(this);

	map->addGameObject(player);

	enemies.push_back(new Enemy(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "Tojic", 100, 100, 0.15, 10, "Churka"));
	enemies[0]->setPosition(8 * 32, 10 * 32);
	enemies[0]->setNewHitboxPosition(sf::Vector2f(4, 8));
	map->addEntity(enemies[0]);
	enemies[0]->setGame(this);

	player->setPosition(6*32, 10*32);

	camera = new Camera(sf::Vector2f(300, 300), player->getObjectPosition(), window);
	window->setView(*camera->getView());

	is_gameover = false;
	delta_time = 0;
	gameover_timer = 0;
	is_paused = false;
	is_running = false;

	gameover_text.setString("Game Over");
	gameover_text.setFillColor(sf::Color::White);
	gameover_text.setCharacterSize(200);
	gameover_text.setFont(TextureContainer::getInstance()->getFont());

	gui = new GUI(player);	
	background = sf::RectangleShape(sf::Vector2f(window->getSize()));
	background.setFillColor(sf::Color(0, 0, 0, 128));
}

Game::~Game() {
	delete map;
	delete menu;
	delete player;
	for (int i = 0; i < enemies.size(); i++) delete enemies[i];
	delete camera;
	delete window;
}

void Game::setIsPaused(bool flag) {
	is_paused = flag;
}

bool Game::isPaused() {
	return is_paused;
}

void Game::restartGame() {
	for (int i = 0; i < enemies.size(); i++) {
		map->removeGameObject(enemies[i]);
		delete enemies[i];
	}
	map->removeGameObject(player);
	delete player;
	delete camera;
	enemies.clear();
	
	player = new Player(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "NES_Slave", 100, 100, 0.15, 20, "Yaroslave");
	player->setNewHitboxPosition(sf::Vector2f(4, 8));
	player->setGame(this);

	map->addGameObject(player);

	enemies.push_back(new Enemy(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "Tojic", 100, 100, 0.15, 10, "Churka"));
	enemies[0]->setPosition(8 * 32, 10 * 32);
	enemies[0]->setNewHitboxPosition(sf::Vector2f(4, 8));
	map->addEntity(enemies[0]);
	enemies[0]->setGame(this);

	player->setPosition(6 * 32, 10 * 32);
	camera = new Camera(sf::Vector2f(300, 300), player->getObjectPosition(), window);
	window->setView(*camera->getView());
	
	is_gameover = false;
	setMenu(new MainMenu(window, this));
	menu->setActive(true);

}

void Game::setMenu(Menu* menu) {
	delete this->menu;
	this->menu = menu;
}

void Game::setIsRunning(bool flag) {
	is_running = flag;
}

Player* Game::getPlayer() {
	return player;
}

Camera* Game::getCamera() {
	return camera;
}

void Game::run() {
	is_running = true;
	while (is_running) {
		update();
		render();
	}
}

void Game::update() {

	delta_time = clock.restart().asSeconds();

	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			is_running = false;
			window->close();
		}
		if (event.type == sf::Event::LostFocus) {
			is_active = false;
		}
		else if (event.type == sf::Event::GainedFocus) {
			is_active = true;
		}
		if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::Escape && typeid(*menu) != typeid(MainMenu) ) {
				menu->setActive(!menu->isActive());
				menu->update(event, camera, delta_time);
				background.setPosition(camera->getView()->getCenter().x - camera->getView()->getSize().x,
					camera->getView()->getCenter().y - camera->getView()->getSize().y);
			}
			else if (event.key.code == sf::Keyboard::C) {
				gui->open(!gui->isActive());
				background.setPosition(camera->getView()->getCenter().x - camera->getView()->getSize().x,
									   camera->getView()->getCenter().y - camera->getView()->getSize().y);
			}
		}
	}

	if (!is_gameover) {
		if (is_active && !menu->isActive()) {

			if (player->getHealthPointsValue() <= 0) {
				is_gameover = true;
				return;
			}

			if (enemies.size() == 0) {
				enemies.push_back(new Enemy(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "Tojic", 100, 100, 0.15, 10, "Churka"));
				enemies[0]->setPosition(8 * 32, 10 * 32);
				enemies[0]->setNewHitboxPosition(sf::Vector2f(4, 8));
				map->addEntity(enemies[0]);
				enemies[0]->setGame(this);

				enemies.push_back(new Enemy(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "Tojic", 100, 100, 0.15, 10, "Churka"));
				enemies[1]->setPosition(8 * 32, 5 * 32);
				enemies[1]->setNewHitboxPosition(sf::Vector2f(4, 8));
				map->addEntity(enemies[1]);
				enemies[1]->setGame(this);
			}

			for (int i = 0; i < enemies.size(); i++) enemies[i]->update(delta_time, player, map);
			
			if (gui->isActive()) {
				gui->update(event, camera, delta_time);
			}
			player->update(event, delta_time, map);
			map->update(player);

			camera->update(sf::Vector2f(player->getObjectPosition().x + player->getGlobalBounds().width / 2,
						   player->getObjectPosition().y + player->getGlobalBounds().height / 2), map);

			window->setView(*camera->getView());
		}
		else if (menu->isActive()) {
			menu->update(event, camera, delta_time);
		}
	}
	else {
		camera->getView()->setSize(sf::Vector2f(window->getSize()));
		window->setView(*camera->getView());
		window->clear();
		gameover_text.setPosition(camera->getView()->getCenter().x - gameover_text.getGlobalBounds().width /2, camera->getView()->getCenter().y - gameover_text.getGlobalBounds().height / 2);
		window->draw(gameover_text);
		window->display();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		restartGame();
	}
}

void Game::render() {
	window->clear();
	if (!is_gameover) {
		if (typeid(*menu) != typeid(MainMenu)) {
			window->draw(*map);
		}
		if (menu->isActive()) {
			window->draw(background);
			window->draw(*menu);
		}

		if (gui->isActive()) {
			window->draw(background);
			camera->setSize(1920, 1080);
			window->draw(*gui);
			camera->setSize(300, 300);
		}
	}
	else {
		window->draw(gameover_text);
	}
	window->display();
}

void Game::removeEnemy(Entity* enemy) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemy == enemies[i]) enemies.erase(enemies.begin() + i);
	}
	map->removeGameObject(enemy);
	delete enemy;
}