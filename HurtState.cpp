#include "HurtState.h"
#include "IdleState.h"
#include "Entity.h"
#include "Game.h"

HurtState::HurtState(Entity* entity, float damage_taken, std::string attack_dir, Map* map) : State(entity) {
	name = "Hurt";
	switch_state_timer = 0;
	this->damage_taken = damage_taken;
	is_damage_received = false;
	entity->setCanSwitchState(false);
	attack_direction = attack_dir;
	this->map = map;
	entity->setIsInFight(true);
}

void HurtState::update(float delta_time) {
	switch_state_timer += delta_time;
	
	sf::Vector2f pos0 = entity->getObjectPosition();
	sf::Vector2f pos = entity->getObjectPosition() + entity->getHitboxPosition();
	std::string dir = entity->getDirection();
	float mapEndX = map->getSize().x * map->getTilesetSize().x;
	float mapEndY = map->getSize().y * map->getTilesetSize().y;

	if (attack_direction == "South") {
		if (pos.y + 40 * delta_time < mapEndY) {
			if (!map->checkCollisionWithMap(sf::Vector2f(pos.x, pos.y + 40 * delta_time), entity)) {
				entity->setPosition(pos0.x, pos0.y + 40 * delta_time);
			}
		}
		entity->setDirection("North");
	}
	else if (attack_direction == "West") {
		if (pos.x - 40 * delta_time > 0) {
			if (!map->checkCollisionWithMap(sf::Vector2f(pos.x - 40 * delta_time, pos.y), entity)) {
				entity->setPosition(pos0.x - 40 * delta_time, pos0.y);
			}
		}
		entity->setDirection("East");
	}
	else if (attack_direction == "North") {
		if (pos.y - 40 * delta_time > 0 ) {
			if (!map->checkCollisionWithMap(sf::Vector2f(pos.x, pos.y - 40 * delta_time), entity)) {
				entity->setPosition(pos0.x, pos0.y - 40 * delta_time);
			}
		}
		entity->setDirection("South");
	}

	else if (attack_direction == "East") {
		if (pos.x + 40 * delta_time < mapEndX) {
			if (!map->checkCollisionWithMap(sf::Vector2f(pos.x + 40 * delta_time, pos.y), entity)) {
				entity->setPosition(pos0.x + 40 * delta_time, pos0.y);
			}
		}
		entity->setDirection("West");
	}


	if (!is_damage_received) {
		is_damage_received = true;
		entity->setHealthPointsValue(entity->getHealthPointsValue() - damage_taken);
		entity->updateHealthBar();
		SoundContainer::getInstance()->getSoundByName(name)->play();
	}

	if (switch_state_timer > 0.5) {
		entity->setCanSwitchState(true);
	}
	
	if (entity->getStaminaBar()) entity->getStaminaBar()->setPosition(pos0.x, pos0.y + 4);
	if (entity->getHealthBar()) entity->getHealthBar()->setPosition(pos0.x , pos0.y + 2);

	entity->updateAnimator(delta_time, this);
}

void HurtState::update(float delta_time, Entity* target, Map* map) {
	update(delta_time);
	if (entity->getHealthPointsValue() <= 0) {
		entity->getGame()->removeEnemy(entity);
		return;
	}
	if (entity->canSwitchState()) {
		entity->setState(new IdleState(entity));
		return;
	}
}

