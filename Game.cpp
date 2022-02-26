#include "Game.h"
#include "TextureContainer/TextureContainer.h"
#include <iostream>
#include "Player.h"
#include "Sounds/SoundContainer.h"

Game::Game() {
	settings.antialiasingLevel = 10;
	window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "YaroslaveTheGame", sf::Style::Default, settings);

	event = sf::Event();
	TextureContainer::getInstance(); 
	SoundContainer::getInstance();
	map = new Map("Map\\Map.txt", "Map\\GameObjectsMap.txt");


	player = new Player(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "NES_Slave", 100, 100, 0.15, 20, "Yaroslave");
	player->setNewHitboxPosition(sf::Vector2f(4, 8));
	player->setGame(this);

	map->addGameObject(player);

	enemies.push_back(new Enemy(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "NES_Slave", 100, 100, 0.15, 10, "Churka"));
	enemies[0]->setPosition(8 * 32, 10 * 32);
	enemies[0]->setNewHitboxPosition(sf::Vector2f(4, 8));
	map->addEntity(enemies[0]);
	enemies[0]->setGame(this);

	player->setPosition(6*32, 10*32);

	camera = new Camera(sf::Vector2f(300, 300), player->getObjectPosition());
	window->setView(*camera->getView());
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
			if (event.key.code == sf::Keyboard::Escape) {
				is_paused = !is_paused;
			}
		}
	}

	player->update(event, delta_time, map);
	for (int i = 0; i < enemies.size(); i++) enemies[i]->update(delta_time, player, map);

	if (enemies.size() == 0) {
		enemies.push_back(new Enemy(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "NES_Slave", 100, 100, 0.15, 10, "Churka"));
		enemies[0]->setPosition(8 * 32, 10 * 32);
		enemies[0]->setNewHitboxPosition(sf::Vector2f(4, 8));
		map->addEntity(enemies[0]);
		enemies[0]->setGame(this);

		enemies.push_back(new Enemy(sf::Vector2f(16, 32), sf::Vector2f(7, 23), "NES_Slave", 100, 100, 0.15, 10, "Churka"));
		enemies[1]->setPosition(8 * 32, 5 * 32);
		enemies[1]->setNewHitboxPosition(sf::Vector2f(4, 8));
		map->addEntity(enemies[1]);
		enemies[1]->setGame(this);
	}

	map->update(player);
	camera->update(sf::Vector2f(player->getObjectPosition().x + player->getGlobalBounds().width / 2,
				   player->getObjectPosition().y + player->getGlobalBounds().height / 2), map);

	window->setView(*camera->getView());
}

void Game::render() {
	window->clear();
	window->draw(*map);
	window->display();
}

void Game::removeEnemy(Entity* enemy) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemy == enemies[i]) enemies.erase(enemies.begin() + i);
	}
	map->removeGameObject(enemy);
	delete enemy;
}