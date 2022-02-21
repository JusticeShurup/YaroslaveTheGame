#include "Player.h"
#include <iostream>
#include "TextureContainer/TextureContainer.h"
#include "IdleState.h"
#include "RunState.h"
#include "WalkState.h"
#include "AttackState.h"
#include "HurtState.h"


Player::Player() : Entity(sf::Vector2f(16, 32), sf::Vector2f(16, 32)) {
	current_xp = 0;
	level = 1;
}

Player::Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name)
	: Entity(entity_size, hitbox_size, textures_name, maxHP, maxStam, speed, damage, name) {
	current_xp = 0;
	level = 1;
	setCanSwitchState(true);
}

void Player::update(sf::Event& event, float dt, Map* map) {
	Entity::update(dt);
	sf::Vector2f pos = getObjectPosition() + getHitboxPosition();
	float speed = getSpeedValue();
	float dx = 0;
	float dy = 0;
	bool is_walking = false;
	bool is_running = false;

	if (canSwitchState()) {

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				dy = speed * dt * (-500);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && getStaminaValue() > 10) {
					dy = speed * dt * (-500) * 1.5;
					if (getState()->getName() != "Run") setState(new RunState(this));
				}
				else {
					if (getState()->getName() != "Walk") setState(new WalkState(this));
				}
				setDirection("North");
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				dy = speed * dt * 500;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && getStaminaValue() > 10) {
					dy = speed * dt * (500) * 1.5;
					if (getState()->getName() != "Run") setState(new RunState(this));
				}
				else {
					if (getState()->getName() != "Walk") setState(new WalkState(this));
				}
				setDirection("South");
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				dx = speed * dt * (-500);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && getStaminaValue() > 10) {
					dx = speed * dt * (-500) * 1.5;
					if (getState()->getName() != "Run") setState(new RunState(this));
				}
				else {
					if (getState()->getName() != "Walk") setState(new WalkState(this));
				}
				setDirection("West");
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				dx = speed * dt * 500;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && getStaminaValue() > 10) {
					dx = speed * dt * (500) * 1.5;
					if (getState()->getName() != "Run") setState(new RunState(this));
				}
				else {
					if (getState()->getName() != "Walk") setState(new WalkState(this));
				}
				setDirection("East");
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && getStaminaValue() > getStamPerAttack() && canSwitchState()) {
			setState(new AttackState(this));

			sf::Vector2i position; // attack range position
			sf::Vector2i size(25, 25); // attack range size
			std::string dir = getDirection();
			if (dir == "North") position = sf::Vector2i(pos.x - size.x / 2, pos.y - size.y);
			else if (dir == "East") position = sf::Vector2i(pos.x, pos.y - size.y / 2);
			else if (dir == "South") position = sf::Vector2i(pos.x - size.x / 2, pos.y);
			else position = sf::Vector2i(pos.x - size.x, pos.y - size.y / 2);

			std::vector<Entity*> entity = map->getEntityInRange(sf::IntRect(position, size));
			for (int i = 0; i < entity.size(); i++) {
				entity[i]->setState(new HurtState(entity[i], getDamageValue(), getDirection()));
			}

		}
		else if (getState()->getName() != "Idle"){
			setState(new IdleState(this));
		}
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