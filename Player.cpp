#include "Player.h"
#include <iostream>
#include "TextureContainer/TextureContainer.h"
#include "States/IdleState.h"
#include "States/RunState.h"
#include "States/WalkState.h"
#include "States/AttackState.h"
#include "States/HurtState.h"


Player::Player() : Entity(sf::Vector2f(16, 32), sf::Vector2f(16, 32)) {
	current_xp = 0;
	level = 1;
	target_lock_timer = 0;
	damage_delivered = false;
	entity_target = nullptr;
	target = nullptr;

}

Player::Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name)
	: Entity(entity_size, hitbox_size, textures_name, maxHP, maxStam, speed, damage, name) {
	current_xp = 0;
	level = 1;
	target_lock_timer = 0;
	damage_delivered = false;
	entity_target = nullptr;
	target = nullptr;

	setCanSwitchState(true);
}

void Player::lockNearestTarget(Map* map) {
	std::vector<Entity*> entity;
	entity = map->getEntity();
	float distance = 1000000;
	int target_number = 0;
	for (int i = 0; i < entity.size(); i++) {
		if (this != entity[i]) {
			if (calcDistance(entity[i]->getCenter(), getCenter()) < distance && calcDistance(entity[i]->getCenter(), getCenter()) <= 300) {
				distance = calcDistance(entity[i]->getCenter(), getCenter());
				target_number = i;
			}
		}
	}
		
	if (distance <= 300) {
		target = new sf::RectangleShape(sf::Vector2f(entity[target_number]->getObjectShape()->getSize().x + 1, entity[target_number]->getObjectShape()->getSize().y + 1));
		target->setFillColor(sf::Color(255, 255, 255, 0));
		target->setOutlineColor(sf::Color::Red);
		target->setOutlineThickness(1);
		target->setPosition(entity[target_number]->getObjectPosition().x - 1, entity[target_number]->getObjectPosition().y - 1);
		entity_target = entity[target_number];
	}
}

void Player::update(sf::Event& event, float dt, Map* map) {
	sf::Vector2f pos = getObjectPosition() + getHitboxPosition();
	float speed = getSpeedValue();
	float dx = 0;
	float dy = 0;
	target_lock_timer += dt;

	if (getState()->getName() == "Attack" && !damage_delivered) {
		sf::Vector2i position; // attack range position
		sf::Vector2i size(30, 30); // attack range size
		std::string dir = getDirection();
		if (dir == "North") position = sf::Vector2i(pos.x + getHitboxShape()->getSize().x / 2 - size.x / 2, pos.y + getHitboxShape()->getSize().y / 2 - size.y);
		else if (dir == "East") position = sf::Vector2i(pos.x, pos.y + getHitboxShape()->getSize().y / 2 - size.y / 2);
		else if (dir == "South") position = sf::Vector2i(pos.x + getHitboxShape()->getSize().x / 2 - size.x / 2, pos.y + size.y);
		else position = sf::Vector2i(pos.x - size.x / 2, pos.y + getHitboxShape()->getSize().y / 2 - size.y / 2);

		std::vector<Entity*> entity = map->getEntityInRange(sf::IntRect(position, size));
		for (int i = 0; i < entity.size(); i++) {
			entity[i]->setState(new HurtState(entity[i], getDamageValue(), getDirection(), map));
			damage_delivered = true;
		}
	}


	if (canSwitchState()) {

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && getStaminaValue() > getStamPerAttack() && canSwitchState()) {
			damage_delivered = false;
			setState(new AttackState(this));

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
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
		else if (getState()->getName() != "Idle"){
			setState(new IdleState(this));
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && target_lock_timer > 0.2) {
		if (!target) {
			lockNearestTarget(map);
		}
		else {
			delete target;
			target = nullptr;
			entity_target = nullptr;
		}
		target_lock_timer = 0;
	}


	if (map->isEntity(entity_target)) {
		target->setPosition(entity_target->getObjectPosition().x - 1, entity_target->getObjectPosition().y - 1);
	}
	else {
		entity_target = nullptr;
		if (target) {
			delete target;
			target = nullptr;
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
	Entity::update(dt);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Entity::draw(target, states);
	if (this->target) {
		target.draw(*this->target);
	}
}

void Player::setDirection(std::string dir) {

	if (!target && !entity_target) { // Если нет цели для атаки
		Entity::setDirection(dir);
	}
	else { // Если есть цель для атаки
		float X = getObjectPosition().x + getHitboxPosition().x; // Координата X - от левого верхнего угла хитбокса
		float Y = getObjectPosition().y + getHitboxPosition().y; // Координата Y - от левого верхнего угла хитбокса
		float targetX = entity_target->getObjectShape()->getPosition().x + entity_target->getHitboxPosition().x;
		float targetY = entity_target->getObjectShape()->getPosition().y + entity_target->getHitboxPosition().y;


		float adjucentLeg = abs(targetX - X); // Прилежащий катет
		float opposingLeg = abs(targetY - Y); // Противолежащий катет

		if (opposingLeg > adjucentLeg) { // Если по Y идти больше чем X, то 
			if (targetY - Y > 0) Entity::setDirection("South"); //Идёт вниз 
			else Entity::setDirection("North"); //Идёт вверх
		}
		else {
			if (targetX - X > 0) Entity::setDirection("East"); //Идёт вправо
			else Entity::setDirection("West"); //Идёт вверх
		}
	}
}