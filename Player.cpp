#include "Player.h"
#include <iostream>
#include "TextureContainer/TextureContainer.h"
#include "States/IdleState.h"
#include "States/RunState.h"
#include "States/WalkState.h"
#include "States/AttackState.h"
#include "States/HurtState.h"
#include "Game.h"

Player::Player() : Entity(sf::Vector2f(16, 32), sf::Vector2f(16, 32)) {
	current_xp = 0;
	level = 1;
	target_lock_timer = 0;
	xp_to_nextlevel = 10;
	
	strength = 1;
	fortitude = 1;
	stamina = 1;
	free_atribute_points = 0;

	inventory = nullptr;
	damage_delivered = false;
	entity_target = nullptr;
	target = nullptr;

}

Player::Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name)
	: Entity(entity_size, hitbox_size, textures_name, maxHP, maxStam, speed, damage, name) {

	current_xp = 0;
	level = 1;

	strength = 1;
	fortitude = 1;
	stamina = 1;
	free_atribute_points = 0;

	xp_to_nextlevel = 10;
	target_lock_timer = 0;

	inventory = new GUI(this);
	damage_delivered = false;
	entity_target = nullptr;
	target = nullptr;
	
	player_name = new sf::Text;
	player_name->setCharacterSize(20);
	player_name->setString("Yaroslave");
	player_name->setFont(TextureContainer::getInstance()->getFont());
	player_name->setFillColor(sf::Color::White);
	player_name->setOutlineColor(sf::Color::Black);
	player_name->setOutlineThickness(2);
	player_name_pos = sf::Vector2f(getObjectPosition());
	player_name->setPosition(player_name_pos);
	player_name_pos.x = 0;
	player_name_pos.y = 0;
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

	inventory->update(event, game->getCamera(), dt);
	
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
		if (getGame()->getCamera()->isCameraLockedY()) {
			player_name_pos.y += dy * (1920 / 300);
		}
		else {
			player_name_pos.y = 0;
		}
		if (pos.y + dy > 0 && pos.y + dy < mapEndY) {
			new_pos.y += dy;
		}
	}
	if (!map->checkCollisionWithMap(sf::Vector2f(pos.x + dx, pos.y), this)) {
		if (getGame()->getCamera()->isCameraLockedX()) {
			player_name_pos.x += dx * (1080 / 300);
		}
		else {
			player_name_pos.x = 0;
		}
		if (pos.x + dx > 0 && pos.x + dx < mapEndX) {
			new_pos.x += dx;
		}
	}

	setPosition(new_pos);
	
	Entity::update(dt);
}

int Player::getXP() const{
	return current_xp;
}

int Player::getXPToNextLevel() const {
	return xp_to_nextlevel;
}

void Player::setXP(int xp) {
	if (xp > this->level * 10) {
		int level;
		for (level = 1; xp > xp_to_nextlevel; level++) {
			xp -= 10 * level;
		}
		current_xp = xp;
		this->level = level;
		free_atribute_points = level - 1;
	}
}

void Player::addXP(int xp) {
	this->current_xp += xp;
	if (current_xp >= xp_to_nextlevel) {
		current_xp %= xp;
		level++;
		free_atribute_points++;
		xp_to_nextlevel = 10 * level;
	}
}

void Player::lostXP(int xp) {
	if (xp > 0) {
		if (current_xp - xp <= 0) {
			current_xp = 0;
		}
		else {
			current_xp -= xp;
		}
	}
}


//Atributes
void Player::addStrength(int points) {
	strength += points;
	setDamageValue(getDamageValue() - 2 * (strength - points) + 2 * strength);
}

int Player::getStrength() const {
	return strength;
}

void Player::addFortitude(int points) {
	fortitude += points;
	setMaxHPValue(getMaxHPValue() - 10 * (fortitude - points) + 10 * fortitude);
}

int Player::getFortitude() const {
	return fortitude;
}

void Player::addStamina(int points) {
	stamina += points;
	setMaxStaminaValue(getMaxStaminaValue() - 10 * (stamina - points) + 10 * stamina);
}

int Player::getStamina() const {
	return stamina;
}

void Player::addFreeAtributePoints(int points) {
	free_atribute_points += points;
}

int Player::getFreeAtributePoints() const {
	return free_atribute_points;
}
//Atributes


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RenderStates text_states;

	getGame()->getCamera()->setSize(1920, 1080);
	text_states.transform = getGame()->getCamera()->getRenderView()->getInverseTransform() * getGame()->getCamera()->getTransform(0 + getGlobalBounds().width * 3, 0 + getGlobalBounds().height * 2.25, 1920, 1080);
	if (inventory->isActive()) target.draw(*inventory, text_states);
	getGame()->getCamera()->setSize(300, 300);
	target.draw(*player_name, text_states);
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