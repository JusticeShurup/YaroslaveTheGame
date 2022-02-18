#include "Player.h"
#include <iostream>
#include "TextureContainer/TextureContainer.h"

Player::Player() : Entity(sf::Vector2f(16, 32), sf::Vector2f(16, 32)) {
	current_xp = 0;
	level = 1;
	can_move = true;
	time_after_attack = 0;
}

Player::Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name)
	: Entity(entity_size, hitbox_size, textures_name, maxHP, maxStam, speed, damage, name) {
	current_xp = 0;
	level = 1;
	can_move = true;
	time_after_attack = 0;
}

void Player::update(sf::Event& event, float dt, Map* map) {
	Entity::update(dt);
	float speed = getSpeedValue();
	float dx = 0;
	float dy = 0;
	bool is_walking = false;
	bool is_running = false;

	sf::Vector2f pos = getObjectPosition() + getHitboxPosition();
	if (can_move) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			is_walking = true;
			dy = speed * dt * (-500);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				dy = speed * dt * (-500) * 1.5;
				is_running = true;
			}
			setDirection("North");
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			is_walking = true;
			dy = speed * dt * 500;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				dy = speed * dt * (500) * 1.5;
				is_running = true;
			}
			setDirection("South");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			is_walking = true;
			dx = speed * dt * (-500);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				dx = speed * dt * (-500) * 1.5;
				is_running = true;
			}
			setDirection("West");
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			is_walking = true;
			dx = speed * dt * 500;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				dx = speed * dt * (500) * 1.5;
				is_running = true;
			}
			setDirection("East");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && can_attack && canLostStam()) {
			can_attack = false;
			is_walking = false;
			is_running = false;
			can_move = false;
		}
	}
	if (!can_attack) setState("Attack");
	else if (is_running) setState("Run");
	else if (is_walking) setState("Walk");
	else if (can_move) setState("Idle");

	if (!can_attack) time_after_attack += dt;

	if (time_after_attack > 0.5) {
		can_move = true;
		can_attack = true;
		time_after_attack = 0;
	}

	float mapEndX = map->getSize().x * map->getTilesetSize().x;
	float mapEndY = map->getSize().y * map->getTilesetSize().y;
	sf::Vector2f new_pos(getObjectPosition());

	if (!map->checkCollisionWithMap(sf::Vector2f(pos.x, pos.y + dy), this)) {
		if (pos.y + dy > 0 && pos.y + dy < mapEndY) {
			new_pos.y += dy;
		}
	}
	if (!map->checkCollisionWithMap(sf::Vector2f(pos.x + dx, pos.y), this)) {
		if (pos.x + dx > 0 && pos.x + dx < mapEndX) {
			new_pos.x += dx;
		}
	}
	setPosition(new_pos);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Entity::draw(target, states);
}