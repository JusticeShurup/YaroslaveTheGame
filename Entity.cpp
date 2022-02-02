#include "Entity.h"
#include "TextureContainer/TextureContainer.h"
#include <iostream>
#include "Animator.h"

Entity::Entity() {
	health_points = 0;
	max_health_points = 0;
	stamina_points = 0;
	max_stamina_points = 0;
	damage = 0;
	speed = 0;
	health_bar = nullptr;
	stamina_bar = nullptr;
	animator = nullptr;

	state = "Idle";
}

Entity::Entity(sf::Vector2f entity_size, sf::Vector2f hitbox_size) : GameObject(entity_size, hitbox_size) {
	health_points = 0;
	max_health_points = 0;
	stamina_points = 0;
	max_stamina_points = 0;
	damage = 0;
	speed = 0;
	health_bar = nullptr;
	stamina_bar = nullptr;
	animator = new Animator(this, "NES_Slave");

	state = "Idle";
}

Entity::Entity(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name) : GameObject(entity_size, hitbox_size){
	
	this->nickname.setFont(TextureContainer::getInstance()->getFont());
	this->nickname.setString(name);
	this->nickname.setFillColor(sf::Color::Black);
	this->nickname.setCharacterSize(10);
	this->nickname.setPosition(getObjectPosition().x, getObjectPosition().y + this->nickname.getGlobalBounds().height);
	setName(name);
	setTexturesName(textures_name);
	animator = new Animator(this, textures_name);

	health_points = maxHP;
	max_health_points = maxHP;
	stamina_points = maxStam;
	max_stamina_points = maxStam;
	
	this->speed = speed;
	this->damage = damage;

	setTexture(TextureContainer::getInstance()->getEntityTexture(textures_name, "Idle", "South", 0));

	health_bar = new sf::RectangleShape(sf::Vector2f(entity_size.x, 5));
	stamina_bar = new sf::RectangleShape(sf::Vector2f(entity_size.x, 5));

	state = "Idle";
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

void Entity::setStaminaValue(int value){
	stamina_points = value;
}

int Entity::getStaminaValue() const{
	return stamina_points;
}

void Entity::setMaxStaminaValue(int value) {
	if (max_stamina_points < value && stamina_points > value) {
		stamina_points = value;
	}
	max_stamina_points = value;
}

int Entity::getMaxStaminaValue() const{
	return max_stamina_points;
}

void Entity::setSpeedValue(float value) {
	speed = value;
}

float Entity::getSpeedValue() {
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
	updateAnimator(delta_time, state);
}

void Entity::updateAnimator(float delta_time, std::string state) {
	animator->update(delta_time);
}

Animator* Entity::getAnimator() {
	return animator;
}

std::string Entity::getState() {
	return state;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	GameObject::draw(target, states);
	target.draw(nickname);
}