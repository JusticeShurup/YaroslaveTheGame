#include "HurtState.h"
#include "IdleState.h"
#include "Entity.h"
#include "Game.h"

HurtState::HurtState(Entity* entity, float damage_taken, std::string attack_dir) : State(entity) {
	name = "Hurt";
	switch_state_timer = 0;
	this->damage_taken = damage_taken;
	is_damage_received = false;
	entity->setCanSwitchState(false);
	attack_direction = attack_dir;
}

void HurtState::update(float delta_time) {
	switch_state_timer += delta_time;

	sf::Vector2f pos = entity->getObjectPosition();
	std::string dir = entity->getDirection();
	
	if (attack_direction == "South") {
		entity->setPosition(pos.x, pos.y + 40 * delta_time);
		entity->setDirection("North");
	}
	else if (attack_direction == "West") {
		entity->setPosition(pos.x - 40 * delta_time, pos.y);
		entity->setDirection("East");
	}
	else if (attack_direction == "North") {
		entity->setPosition(pos.x, pos.y - 40 * delta_time);
		entity->setDirection("South");
	}

	else if (attack_direction == "East") {
		entity->setPosition(pos.x + 40 * delta_time, pos.y);
		entity->setDirection("West");
	}


	if (!is_damage_received) {
		is_damage_received = true;
		entity->setHealthPointsValue(entity->getHealthPointsValue() - damage_taken);
		entity->updateHealthBar();
	}

	if (switch_state_timer > 0.5) {
		entity->setCanSwitchState(true);
	}
	
	if (entity->getStaminaBar()) entity->getStaminaBar()->setPosition(pos.x + (entity->getDirection() == "East" ? 0.5 : -0.5), pos.y + 4 + (entity->getDirection() == "South" ? 0.25 : -0.25));
	if (entity->getHealthBar()) entity->getHealthBar()->setPosition(pos.x + (entity->getDirection() == "East" ? 0.5 : -0.5), pos.y + 2 + (entity->getDirection() == "South" ? 0.25 : -0.25));

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

