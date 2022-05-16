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
	
	dx = 0;
	dy = 0;

	strength = 1;
	fortitude = 1;
	stamina = 1;
	free_atribute_points = 0;

	damage_delivered = false;
	entity_target = nullptr;
	target = nullptr;

}

Player::Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name, GUI* gui)
	: Entity(entity_size, hitbox_size, textures_name, maxHP, maxStam, speed, damage, name) {

	current_xp = 0;
	level = 1;

	dx = 0;
	dy = 0;

	strength = 1;
	fortitude = 1;
	stamina = 1;
	free_atribute_points = 0;

	xp_to_nextlevel = 10;
	target_lock_timer = 0;

	damage_delivered = false;
	entity_target = nullptr;
	target = nullptr;
	
	player_name = new sf::Text;
	player_name->setCharacterSize(20);
	player_name->setString(name);
	player_name->setFont(TextureContainer::getInstance()->getFont());
	player_name->setFillColor(sf::Color::White);
	player_name->setOutlineColor(sf::Color::Black);
	player_name->setOutlineThickness(2);
	setCanSwitchState(true);

	this->gui = gui;
}

void Player::setGUI(GUI* gui) {
	this->gui = gui;
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

/*
void Player::keyboardUpdate(sf::Event& event, float dt, Map* map) {

	if (canSwitchState()) {

		if (event.key.code == sf::Keyboard::Space && getStaminaValue() > getStamPerAttack() && canSwitchState()) {
			damage_delivered = false;
			setState(new AttackState(this));
		}

		else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D) {
			if (event.key.code == sf::Keyboard::W) {
				dy = speed * dt * (-500);
				if (event.key.code == sf::Keyboard::LShift && getStaminaValue() > 10) {
					dy = speed * dt * (-500) * 1.5;
					if (getState()->getName() != "Run") setState(new RunState(this));
				}
				else {
					if (getState()->getName() != "Walk") setState(new WalkState(this));
				}
				setDirection("North");
			}
			else if (event.key.code == sf::Keyboard::S) {
				dy = speed * dt * 500;
				if (event.key.code == sf::Keyboard::LShift && getStaminaValue() > 10) {
					dy = speed * dt * (500) * 1.5;
					if (getState()->getName() != "Run") setState(new RunState(this));
				}
				else {
					if (getState()->getName() != "Walk") setState(new WalkState(this));
				}
				setDirection("South");
			}
			if (event.key.code == sf::Keyboard::A) {
				dx = speed * dt * (-500);
				if (event.key.code == sf::Keyboard::LShift && getStaminaValue() > 10) {
					dx = speed * dt * (-500) * 1.5;
					if (getState()->getName() != "Run") setState(new RunState(this));
				}
				else {
					if (getState()->getName() != "Walk") setState(new WalkState(this));
				}
				setDirection("West");
			}
			else if (event.key.code == sf::Keyboard::D) {
				dx = speed * dt * 500;
				if (event.key.code == sf::Keyboard::LShift && getStaminaValue() > 10) {
					dx = speed * dt * (500) * 1.5;
					if (getState()->getName() != "Run") setState(new RunState(this));
				}
				else {
					if (getState()->getName() != "Walk") setState(new WalkState(this));
				}
				setDirection("East");
			}
		}
		else if (getState()->getName() != "Idle") {
			setState(new IdleState(this));
		}
	}

}
*/

void Player::update(sf::Event& event, float dt, Map* map) {
	float speed = getSpeedValue();

	sf::Vector2f pos = getObjectPosition() + getHitboxPosition();
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

	if (getState()->getName() == "Hurt") {
		gui->setActive(false);
	}

	if (canSwitchState() && !gui->isActive()) {



		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && getStaminaValue() > getStamPerAttack() && canSwitchState()) {
			damage_delivered = false;
			setState(new AttackState(this));
			getGame()->getServer()->updatePosition();
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
			getGame()->getServer()->updatePosition();
		}
		else if (getState()->getName() != "Idle") {
			setState(new IdleState(this));
			getGame()->getServer()->updatePosition();
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && target_lock_timer > 0.2) {
		if (!target) {
			lockNearestTarget(map);
			if (getState()->getName() != "Attack") {
				setDirection();

			}
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
	dx = 0;
	dy = 0;

	for (int i = 0; i < text_animation_continue.size(); i++) {
		if (text_animation_continue[i]) {
			text_animation_continue.erase(text_animation_continue.begin() + i);
			elapsed_time.erase(elapsed_time.begin() + i);
			delete xp_texts[i];
			xp_texts.erase(xp_texts.begin() + i);
		}
	}
	for (int i = 0; i < xp_texts.size(); i++) {
		elapsed_time[i] += dt;
		updateText(1, elapsed_time[i], dt, xp_texts[i], text_animation_continue, i);
	}
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
	xp_texts.push_back(initXPText("+" + std::to_string(xp) + " XP", sf::Vector2f(getObjectPosition().x, getObjectPosition().y + 20)));
	elapsed_time.push_back(0);
	text_animation_continue.push_back(false);
	if (current_xp >= xp_to_nextlevel) {
		current_xp %= xp;
		level++;
		SoundContainer::getInstance()->getSoundByName("LevelUp")->play();
		xp_texts.push_back(initXPText("Level UP!", getObjectPosition()));
		elapsed_time.push_back(0);
		text_animation_continue.push_back(false);
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
void Player::addAtributeByName(std::string name, int points) {
	if (name == "Strength") {
		strength += points;
		setDamageValue(20 + 2 * (strength - 1));
	}
	else if (name == "Fortitude") {
		fortitude += points;
		setMaxHPValue(100 + 10 * (fortitude - 1));
	}
	else if (name == "Stamina") {
		stamina += points;
		setMaxStaminaValue(100 + 10 * (stamina - 1));
	}

}

int Player::getAtributeByName(std::string name) {
	if (name == "Strength") return strength;
	else if (name == "Fortitude") return fortitude;
	else if (name == "Stamina") return stamina;
}

void Player::addStrength(int points) {
	strength += points;
	setDamageValue(20 + 2 * (strength - 1));
}

int Player::getStrength() const {
	return strength;
}

void Player::addFortitude(int points) {
	fortitude += points;
	setMaxHPValue(100 + 10 * (fortitude - 1));
}

int Player::getFortitude() const {
	return fortitude;
}

void Player::addStamina(int points) {
	stamina += points;
	setMaxStaminaValue(100 + 10 * (stamina - 1));
}

int Player::getStamina() const {
	return stamina;
}

void Player::setFreeAtributePoints(int points) {
	free_atribute_points = points;
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
	Entity::draw(target, states);
	if (this->target) {
		target.draw(*this->target);
	}
	for (int i = 0; i < xp_texts.size(); i++) {
		getGame()->getCamera()->setSize(1920, 1080);
		target.draw(*xp_texts[i]);
		getGame()->getCamera()->setSize(300, 300);
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

sf::Text* Player::initXPText(std::string string, sf::Vector2f pos) {
	sf::Text* text = new sf::Text();
	text->setFont(TextureContainer::getInstance()->getFont());
	text->setString(string);
	text->setCharacterSize(20);
	text->setFillColor(sf::Color::White);
	text->setOutlineColor(sf::Color::Black);
	text->setOutlineThickness(3);
	text->setPosition(pos);
	return text;
}

void Player::updateText(float anim_time, float elapsed_time, float delta_time, sf::Text* text, std::vector<bool>& flags, int number) {

	sf::Vector2f pos = text->getPosition(); 

	text->setPosition(pos.x, pos.y - 100 * delta_time);
	
	if (elapsed_time >= anim_time) {
		flags[number] = true;
	}
}