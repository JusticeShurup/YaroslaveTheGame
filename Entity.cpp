#include "Entity.h"
#include "TextureContainer/TextureContainer.h"
#include <iostream>
#include "Animator.h"

Entity::Entity() {
	health_points = 0;
	max_health_points = 0;
	stamina_points = 0;
	
	max_stamina_points = 0;
	stam_per_tick = 0;
	stam_per_attack = 0;
	lost_stam_timer = 0;
	add_stam_timer = 0;
	can_lost_stam = false;

	damage = 0;
	speed = 0;
	health_bar = nullptr;
	stamina_bar = nullptr;
	animator = nullptr;
	
	switch_state_timer = 0;
	can_switch_state = true;

	state = "Idle";
	direction = "South";
}

Entity::Entity(sf::Vector2f entity_size, sf::Vector2f hitbox_size) : GameObject(entity_size, hitbox_size) {
	health_points = 0;
	max_health_points = 0;

	stamina_points = 0;
	max_stamina_points = 0;
	stam_per_tick = 0;
	stam_per_attack = 0;
	lost_stam_timer = 0;
	add_stam_timer = 0;
	can_lost_stam = false;

	damage = 0;
	speed = 0;
	health_bar = new sf::RectangleShape(sf::Vector2f(entity_size.x, 1.5));
	health_bar->setFillColor(sf::Color::Red);
	stamina_bar = new sf::RectangleShape(sf::Vector2f(entity_size.x, 1.5));
	stamina_bar->setFillColor(sf::Color::Green);
	animator = new Animator(this, "NES_Slave");

	switch_state_timer = 0;
	can_switch_state = true;

	state = "Idle";
	direction = "South";
}

Entity::Entity(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name) : GameObject(entity_size, hitbox_size){
           
	this->nickname.setFont(TextureContainer::getInstance()->getFont());                                               
	this->nickname.setString(name);                                                               
	this->nickname.setFillColor(sf::Color::White);
	this->nickname.setOutlineThickness(1);
	this->nickname.setOutlineColor(sf::Color::Black);
	this->nickname.setCharacterSize(5);
	this->nickname.setPosition(getObjectPosition().x, getObjectPosition().y + this->nickname.getGlobalBounds().height);

	setName(name);
	setTexturesName(textures_name);
	animator = new Animator(this, textures_name);

	health_points = maxHP;
	max_health_points = maxHP;

	stamina_points = maxStam;
	max_stamina_points = maxStam;
	stam_per_tick = 1;
	stam_per_attack = 10;
	lost_stam_timer = 0;
	add_stam_timer = 0;
	can_lost_stam = true;

	this->speed = speed;
	this->damage = damage;

	setTexture(TextureContainer::getInstance()->getEntityTexture(textures_name, "Idle", "South", 0));

	health_bar = new sf::RectangleShape(sf::Vector2f(entity_size.x, 1.5));
	health_bar->setFillColor(sf::Color::Red);
	health_bar->setOutlineColor(sf::Color(128, 128, 128));
	health_bar->setOutlineThickness(0.25);
	stamina_bar = new sf::RectangleShape(sf::Vector2f(entity_size.x, 1.5));
	stamina_bar->setFillColor(sf::Color::Green);
	stamina_bar->setOutlineColor(sf::Color(128, 128, 128));
	stamina_bar->setOutlineThickness(0.25);

	switch_state_timer = 0;
	can_switch_state = true;

	state = "Idle";
	direction = "South";
}

Entity::~Entity() {
	delete health_bar;
	delete stamina_bar;
}

//Name
void Entity::setName(std::string name) {
	GameObject::setName(name);
	this->nickname.setString(name);
}

std::string Entity::getName() const {
	return nickname.getString();
}
//Name


sf::FloatRect Entity::getGlobalBounds() {
	return getObjectShape()->getGlobalBounds();
}

void Entity::setTexturesName(std::string name) {
	GameObject::setName(name);
}

std::string Entity::getTexturesName() {
	return GameObject::getName();
}

void Entity::setTexture(std::string filename) {
	GameObject::setTexture(filename);
}

void Entity::setTexture(sf::Texture* texture) {
	GameObject::setTexture(texture);
}

//Specifications
void Entity::setHealthPointsValue(int value) {
	health_points = value;
}

int Entity::getHealthPointsValue() const {
	return health_points;
}

void Entity::setMaxHPValue(int value) {
	max_health_points = value;
}

int Entity::getMaxHPValue() const {
	return max_health_points;
}

void Entity::setStaminaValue(int value) {
	stamina_points = value;
}

int Entity::getStaminaValue() const {
	return stamina_points;
}

void Entity::setMaxStaminaValue(int value) {
	if (max_stamina_points < value && stamina_points > value) {
		stamina_points = value;
	}
	max_stamina_points = value;
}

int Entity::getMaxStaminaValue() const {
	return max_stamina_points;
}

float Entity::getStamPerTick() const {
	return stam_per_tick;
}

int Entity::getStamPerAttack() const {
	return stam_per_attack;
}

bool Entity::canLostStam() const {
	return can_lost_stam;
}

void Entity::setSpeedValue(float value) {
	speed = value;
}

float Entity::getSpeedValue() const{
	return speed;
}

void Entity::setDamageValue(int value) {
	damage = value;
}

int Entity::getDamageValue() {
	return damage;
}
//Specifications

void Entity::update(float delta_time) {
	lost_stam_timer += delta_time; 
	add_stam_timer += delta_time;
	float stam = stamina_points;
	if(!can_switch_state)switch_state_timer += delta_time;


	sf::Vector2f pos = getObjectPosition();
	nickname.setPosition(pos - sf::Vector2f(nickname.getGlobalBounds().width / 2 - getGlobalBounds().width / 2, 5));
	if (state == "Run" && lost_stam_timer > 0.2 && stam - stam_per_tick > 0) {
		stam -= stam_per_tick;
		lost_stam_timer = 0;

	}
	if ((state == "Idle" || state == "Walk") && add_stam_timer > 0.2 && stam + stam_per_tick < max_stamina_points) {
		stam += stam_per_tick;
		add_stam_timer = 0;
	}
	if (state == "Attack" && can_lost_stam && stam > stam_per_attack) {
		stam -= stam_per_attack;
		can_lost_stam = false;
		can_switch_state = false;
	}

	if (switch_state_timer > 0.5) {
		switch_state_timer = 0;
		can_switch_state = true;
	}

	if (stamina_points > stam_per_attack && state != "Attack") can_lost_stam = true;

	stamina_points = stam;

	stamina_bar->setSize(sf::Vector2f((getObjectShape()->getSize().x / max_stamina_points) * stamina_points, stamina_bar->getSize().y));
	health_bar->setSize(sf::Vector2f((getObjectShape()->getSize().x / max_health_points) * health_points, health_bar->getSize().y));

	if (stamina_bar) stamina_bar->setPosition(pos.x + (direction == "East" ? 0.5 : -0.5), pos.y + 4 + (direction == "South" ? 0.25 : -0.25));
	if (health_bar) health_bar->setPosition(pos.x + (direction == "East" ? 0.5 : -0.5), pos.y + 2 + (direction == "South" ? 0.25 : -0.25));
	updateAnimator(delta_time, state);
}

void Entity::updateAnimator(float delta_time, std::string state) {
	animator->update(delta_time);
}

//Animator

Animator* Entity::getAnimator() {
	return animator;
}

std::string Entity::getState() {
	return state;
}

void Entity::setState(std::string state) {
	if (state == "Run") {
		if (stamina_points > 0) {
			this->state = "Run";
		}
		else {
			this->state = "Idle";
		}
		return;
	}
	if (state == "Attack") {
		if (stamina_points > 10) {
			this->state = "Attack";
		}
		else if(can_switch_state) {
			this->state = "Idle";
		}
		return;
	}
	this->state = state;
}

std::string Entity::getDirection() {
	return direction;
}

void Entity::setDirection(std::string dir) {
	direction = dir;
}
//Animator

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	GameObject::draw(target, states);
	target.draw(nickname);
	if (health_bar) target.draw(*health_bar);
	if (stamina_bar) target.draw(*stamina_bar);
}