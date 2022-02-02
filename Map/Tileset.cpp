#include "Tileset.h"
#include <iostream>

Tileset::Tileset() {
	shape = new sf::RectangleShape(sf::Vector2f(32, 32));
	shape->setOutlineColor(sf::Color::Green);
	texture = nullptr;
	temp_texture = nullptr;
	sost = 1;
}

Tileset::Tileset(sf::Vector2f position) {
	shape = new sf::RectangleShape(sf::Vector2f(32, 32));
	shape->setPosition(position);
	shape->setOutlineColor(sf::Color::Green);
	texture = nullptr;
	temp_texture = nullptr;
	sost = 1;
}

Tileset::Tileset(std::string filename, std::string sign, sf::Vector2f size, sf::Vector2f position, uint8_t sost) {
	shape = new sf::RectangleShape(size);
	shape->setOutlineColor(sf::Color::Green);
	this->sign = sign;
	setTexture(filename);
	setPosition(position);
	setSost(sost);
}

Tileset::Tileset(sf::Texture* texture, std::string sign, sf::Vector2f size, sf::Vector2f position, uint8_t sost) {
	shape = new sf::RectangleShape(size);
	shape->setOutlineColor(sf::Color::Green);
	this->sign = sign;
	setPosition(position);
	setTexture(texture);
	setTempTexture(texture);
	setSost(sost);
}

Tileset::~Tileset() {
}

void Tileset::copyTexture(sf::Texture* texture) {
	sf::Image image;
	image = texture->copyToImage();
	const sf::Uint8* pixels = image.getPixelsPtr();

	this->texture->update(pixels);
}

void Tileset::setImage(sf::Texture* texture) {
	image = texture->copyToImage();
}

void Tileset::setSign(std::string sign) {
	this->sign = sign;
}

std::string Tileset::getSign() {
	return sign;
}

void Tileset::setTexture(std::string filename) {
	if (filename != "null") { 
		texture = new sf::Texture();
		texture->loadFromFile(filename);
		shape->setTexture(texture);
	}
	else {
		shape->setTexture(nullptr);
	}
}

void Tileset::setTexture(sf::Texture* texture) {
	this->texture = texture;
 	shape->setTexture(this->texture);
}

void Tileset::setTempTexture(sf::Texture* texture) {
	this->temp_texture = texture;
}

sf::Texture* Tileset::getTempTexture() {
	return temp_texture;
}

sf::Texture* Tileset::getTexture() {
	return texture;
}

sf::Image Tileset::getImage() {
	return image;
}

void Tileset::setSost(uint8_t sost) {
	if (sost > 4) {
		sost = 1;
	}
	else if (sost < 1) {
		sost = 4;
	}
	this->sost = sost;
}

uint8_t Tileset::getSost() {
	return sost;
}

void Tileset::setPosition(float x, float y) {
	shape->setPosition(sf::Vector2f(x, y));
}

void Tileset::setPosition(sf::Vector2f pos) {
	shape->setPosition(pos);
}

sf::Vector2f Tileset::getPosition() {
	return shape->getPosition();
}

sf::RectangleShape* Tileset::getShape() {
	return shape;
}

void Tileset::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*shape);
}