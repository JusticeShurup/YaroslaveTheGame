#include "Game.h"
#include "TextureContainer/TextureContainer.h"
#include <iostream>
#include "Player.h"

Game::Game() {
	settings.antialiasingLevel = 10;
	window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "YaroslaveTheGame", sf::Style::Default, settings);

	TextureContainer::getInstance(); 
	map = new Map("D:\\VisualStudio2019\\YaroslaveTheGame\\Map\\Map.txt", "D:\\VisualStudio2019\\YaroslaveTheGame\\Map\\GameObjectsMap.txt");


	player = new Player(sf::Vector2f(16, 32), sf::Vector2f(16, 32), "NES_Slave", 100, 100, 0.15, 100, "Yaroslave");
	entity.push_back(player);
	entity[0]->setPosition(2*32, 10*32);
	map->addGameObject(entity[0]);

	camera = new Camera(sf::Vector2f(300, 300), entity[0]->getObjectPosition());
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
	player->update(event, delta_time);


	camera->setPosition(entity[0]->getObjectPosition().x + entity[0]->getGlobalBounds().width / 2, 
						entity[0]->getObjectPosition().y + entity[0]->getGlobalBounds().height / 2);
	window->setView(*camera->getView());
}

void Game::render() {
	window->clear();
	window->draw(*map);
	window->display();
}