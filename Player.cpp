#include "Player.h"
#include <iostream>

Player::Player() : Entity(sf::Vector2f(16, 32), sf::Vector2f(16, 32)) {
	current_xp = 0;
	level = 1;
}

Player::Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name)
	: Entity(entity_size, hitbox_size, textures_name, maxHP, maxStam, speed, damage, name) {
	current_xp = 0;
	level = 1;
}

void Player::update(sf::Event& event, float dt) {
	Entity::update(dt);
	float speed = getSpeedValue();
	float dx = 0;
	float dy = 0;
	sf::Vector2f pos = getObjectPosition();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		dy = speed * dt * (-500);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		dy = speed * dt * 500;

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		dx = speed * dt * (-500);

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		dx = speed * dt * 500;
	}
	setPosition(pos.x + dx, pos.y + dy);
}