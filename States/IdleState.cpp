#include "IdleState.h"
#include "WalkState.h"
#include "RunState.h"
#include "AttackState.h"
#include "../Entity.h"

IdleState::IdleState(Entity* entity) :
	State(entity) {
	add_stam_timer = 0;
	name = "Idle";
	if (entity->getStaminaBar()) entity->getStaminaBar()->setPosition(entity->getObjectPosition().x, entity->getObjectPosition().y + 4);
	if (entity->getHealthBar()) entity->getHealthBar()->setPosition(entity->getObjectPosition().x, entity->getObjectPosition().y + 2);
}

void IdleState::update(float delta_time) {
	add_stam_timer += delta_time;


	if (add_stam_timer > 0.2 && entity->getStaminaValue() + entity->getStamPerTick() * 2 < entity->getMaxStaminaValue()) {
		entity->setStaminaValue(entity->getStaminaValue() + entity->getStamPerTick() * 2);
		add_stam_timer = 0;
		entity->updateStaminaBar();
	}
	else if(add_stam_timer > 0.2) {
		entity->setStaminaValue(entity->getMaxStaminaValue());
	}

	if (entity->getStaminaBar()) entity->getStaminaBar()->setPosition(entity->getObjectPosition().x, entity->getObjectPosition().y + 4);
	if (entity->getHealthBar()) entity->getHealthBar()->setPosition(entity->getObjectPosition().x, entity->getObjectPosition().y + 2);

	entity->updateAnimator(delta_time, this);
}

void IdleState::update(float delta_time, Entity* target, Map* map) {
	entity->update(delta_time);
	entity->updateAnimator(delta_time, this);
	float distance = entity->calcDistance(target->getObjectPosition() + target->getHitboxPosition());

	int distX = entity->getHitboxShape()->getSize().x + 1; // Расстояние, при котором враг уже сталкивается с объектом по X
	int distY = entity->getHitboxShape()->getSize().y + 1; // Расстояние, при котором враг уже сталкивается с объектом по X

	if (distance > 100 && distance < 300 && entity->getStaminaValue() >= 10) {
		entity->setState(new RunState(entity));
		return;
	}
	else if (distance > 22 && distance < 100) {
		entity->setState(new WalkState(entity));
	}
	else if (distance < 22 && entity->getStaminaValue() >= entity->getStamPerAttack()) {
		entity->setState(new AttackState(entity));
	}

}