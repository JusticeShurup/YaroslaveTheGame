#include "AttackState.h"
#include "IdleState.h"
#include "HurtState.h"
#include "Entity.h"

AttackState::AttackState(Entity* entity) : State(entity){
	switch_state_timer = 0;
	is_stamina_lost = false;
	name = "Attack";
}

void AttackState::update(float delta_time) {
	switch_state_timer += delta_time;
	if (entity->getStaminaValue() >= entity->getStamPerAttack() && !is_stamina_lost) {
		entity->setStaminaValue(entity->getStaminaValue() - entity->getStamPerAttack());
		entity->updateStaminaBar();
		is_stamina_lost = true;
		entity->setCanSwitchState(false);
	}
	if (switch_state_timer > 0.5) {
		entity->setCanSwitchState(true);
		entity->setState(new IdleState(entity));
		return;
	}
	entity->updateAnimator(delta_time, this);
}

void AttackState::update(float delta_time, Entity* target, Map* map) {
	if (!is_stamina_lost) {
		sf::Vector2f pos = entity->getObjectPosition() + entity->getHitboxPosition();

		sf::Vector2i position; // attack range position
		sf::Vector2i size(25, 25); // attack range size
		std::string dir = entity->getDirection();
		if (dir == "North") position = sf::Vector2i(pos.x - size.x / 2, pos.y - size.y + entity->getObjectShape()->getSize().y);
		else if (dir == "East") position = sf::Vector2i(pos.x, pos.y - size.y / 2);
		else if (dir == "South") position = sf::Vector2i(pos.x - size.x / 2, pos.y);
		else position = sf::Vector2i(pos.x + entity->getObjectShape()->getSize().x - size.x, pos.y - size.y / 2);

		sf::IntRect range(position, size);

		if (range.contains(sf::Vector2i(target->getObjectPosition() + target->getHitboxPosition()))) {
			target->setState(new HurtState(target, entity->getDamageValue()));
		}
	}
	update(delta_time);
}