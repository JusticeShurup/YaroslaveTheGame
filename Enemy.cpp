#include "Enemy.h"
#include <math.h>
#include <thread>
#include <iostream>
#include "States/IdleState.h"
#include "Game.h"

Enemy::Enemy(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name) :
	Entity(entity_size, hitbox_size, textures_name, maxHP, maxStam, speed, damage, name) {
	can_attack = true;
	can_move = true;

	walk_only_X = false;
	walk_only_Y = false;

	dist_x = 0;
	dist_y = 0;

	setState(new IdleState(this));

}

Enemy::~Enemy() {
	game->getPlayer()->addXP(10);
}

void Enemy::update(float delta_time, Entity* target, Map* map) {
	getState()->update(delta_time, target, map);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Entity::draw(target, states);
}