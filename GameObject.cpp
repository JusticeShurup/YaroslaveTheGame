#include "GameObject.h"
#include <iostream>

GameObject::GameObject() : name("") {
	object = new sf::RectangleShape();
	object->setPosition(0, 0);
	texture = nullptr;
	hitboxes.emplace_back(sf::RectangleShape());
	hitboxes[0].setFillColor(sf::Color(255, 255, 255, 128));
	hitboxes[0].setOutlineColor(sf::Color::Red);
	hitbox_delta_positions.push_back(sf::Vector2f(0, 0));
	is_load_from_file = false;
	is_hitboxes_hidden = true;
}

GameObject::GameObject(sf::Vector2f obj_size, sf::Vector2f hitbox_size) : name("") {
	object = new sf::RectangleShape(obj_size);
	object->setPosition(0, 0);
	texture = nullptr;
	hitboxes.push_back(sf::RectangleShape(hitbox_size));
	hitbox_delta_positions.push_back(sf::Vector2f(0, 0));
	is_load_from_file = false;
	is_hitboxes_hidden = true;
}

GameObject::GameObject(std::string filename, sf::Vector2f obj_size, sf::Vector2f obj_pos, sf::Vector2f hitbox_size, sf::Vector2f hitbox_pos, std::string name) {
	this->name = name;

	is_load_from_file = true;
	texture = new sf::Texture;
	texture->loadFromFile(filename);

	object = new sf::RectangleShape(obj_size);
	object->setOutlineColor(sf::Color::Green);
	
	hitboxes.push_back(sf::RectangleShape(hitbox_size));
	hitboxes[0].setFillColor(sf::Color(255, 255, 255, 128));
	hitboxes[0].setOutlineColor(sf::Color::Red);
	hitbox_delta_positions.push_back(hitbox_pos);

	object->setTexture(texture);
	setPosition(obj_pos);
}

GameObject::GameObject(sf::Texture* texture, sf::Vector2f obj_size, sf::Vector2f obj_pos, sf::Vector2f hitbox_size, sf::Vector2f hitbox_pos, std::string name) {
	this->name = name;
	is_load_from_file = false;

	object = new sf::RectangleShape(obj_size);
	object->setOutlineColor(sf::Color::Green);

	hitboxes.push_back(sf::RectangleShape(hitbox_size));
	hitboxes[0].setFillColor(sf::Color(255, 255, 255, 128));
	hitboxes[0].setOutlineColor(sf::Color::Red);
	hitbox_delta_positions.push_back(hitbox_pos);

	this->texture = texture;
	object->setTexture(texture);
	setPosition(obj_pos);
}

GameObject::GameObject(sf::Texture* texture, sf::Vector2f obj_size, sf::Vector2f obj_pos, int hitbox_count, std::vector<sf::Vector2f> hitbox_sizes, std::vector<sf::Vector2f> hitbox_positions, std::string name) {
	this->name = name;
	is_load_from_file = false;


	object = new sf::RectangleShape(obj_size);
	object->setOutlineColor(sf::Color::Green);

	for (int i = 0; i < hitbox_count; i++) {
		hitboxes.push_back(sf::RectangleShape(hitbox_sizes[i]));
		hitboxes[i].setFillColor(sf::Color(255, 255, 255, 128));
		hitboxes[i].setOutlineColor(sf::Color::Red);
		hitboxes[i].setOutlineThickness(1);
		hitbox_delta_positions.push_back(hitbox_positions[i]);
	}

	this->texture = texture;
	object->setTexture(texture, 1);
	setPosition(obj_pos);
}

GameObject::~GameObject() {
	if (is_load_from_file) delete texture;
}

sf::RectangleShape* GameObject::getObjectShape() {
	return object;
}

sf::RectangleShape* GameObject::getHitboxShape() {
	return &hitboxes[0];
}

sf::RectangleShape* GameObject::getHitboxShape(int number) {
	return &hitboxes[number];
}

std::vector<sf::RectangleShape>* GameObject::getHitboxShapes() {
	return &hitboxes;
}

int GameObject::getHitboxCount() {
	return hitboxes.size();
}

void GameObject::setPosition(sf::Vector2f obj_pos) {
	object->setPosition(obj_pos);
	for (int i = 0; i < hitboxes.size(); i++) {
		hitboxes[i].setPosition(obj_pos.x + hitbox_delta_positions[i].x, obj_pos.y + hitbox_delta_positions[i].y);
	}
}

void GameObject::setPosition(float x, float y) {
	object->setPosition(x, y); 
	for (int i = 0; i < hitboxes.size(); i++) {
		hitboxes[i].setPosition(x + hitbox_delta_positions[i].x, y + hitbox_delta_positions[i].y);
	}
}

void GameObject::setNewHitboxPosition(sf::Vector2f hitbox_pos) {
	hitbox_delta_positions[0] = hitbox_pos;
}

void GameObject::setNewHitboxPositions(std::vector<sf::Vector2f> hitbox_positions) {
	hitbox_delta_positions.clear();
	for (int i = 0; i < hitbox_positions.size(); i++) {
		hitbox_delta_positions.push_back(hitbox_positions[i]);
	}
}

sf::FloatRect GameObject::getGlobalBounds() const{
	return object->getGlobalBounds();
}

sf::Vector2f GameObject::getObjectPosition() const{
	return object->getPosition();
}

sf::Vector2f GameObject::getHitboxPosition(){
	return hitbox_delta_positions[0];
}

sf::Vector2f GameObject::getHitboxPosition(int number) {
	return hitbox_delta_positions[number];
}

std::vector<sf::Vector2f> GameObject::getHitboxPositions() {
	return hitbox_delta_positions;
}

sf::Vector2f GameObject::getCenter() const{
	return sf::Vector2f(getObjectPosition().x + getGlobalBounds().width / 2, getObjectPosition().y + getGlobalBounds().height / 2);
}

void GameObject::showObjectOutlines() {
	if (round(object->getOutlineThickness()) == 0)
		object->setOutlineThickness(3);
	else 
		object->setOutlineThickness(0);
}

void GameObject::showHitboxOutlines() {
	is_hitboxes_hidden = !is_hitboxes_hidden;
	for (int i = 0; i < hitboxes.size(); i++) {
		if (round(hitboxes[i].getOutlineThickness()) == 0)
			hitboxes[i].setOutlineThickness(1);
		else
			hitboxes[i].setOutlineThickness(0);
	}
}

void GameObject::setTexture(std::string filename) {
	if (is_load_from_file) {
		delete texture;
		texture = new sf::Texture;
	}
	if (!texture) texture = new sf::Texture;
	this->texture->loadFromFile(filename);
	object->setTexture(texture, 1);

	is_load_from_file = true;
}

void GameObject::setTexture(sf::Texture* texture) {
	if (is_load_from_file) delete this->texture;
	this->texture = texture;
	object->setTexture(this->texture, 1);
}

sf::Texture* GameObject::getTexture() {
	return texture;
}

void GameObject::setName(std::string name) {
	this->name = name;
}

std::string GameObject::getName() {
	return name;
}

void GameObject::hideHitboxes(bool flag) {
	is_hitboxes_hidden = flag;
}

bool GameObject::getHitboxesHidden() {
	return is_hitboxes_hidden;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*object);
	if (!is_hitboxes_hidden) {
		for (int i = 0; i < hitboxes.size(); i++) {
			target.draw(hitboxes[i]);
		}
	}
}