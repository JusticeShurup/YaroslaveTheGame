#include "Nickname.h"
#include <iostream>
#include "TextureContainer/TextureContainer.h"

Nickname::Nickname() {};

Nickname::Nickname(std::string name) : nickname(name){
	shape.setSize(sf::Vector2f(57, 32));
	initNicknameTexture();
}

void Nickname::setString(std::string name) {
	nickname = name;
	initNicknameTexture();
}

std::string Nickname::getString() const{
	return nickname;
}

void Nickname::setPosition(float x, float y) {
	shape.setPosition(x, y);
}

sf::Vector2f Nickname::getPosition() {
	return shape.getPosition();
}

sf::RectangleShape& Nickname::getShape() {
	return shape;
}

void Nickname::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(shape);
}

void Nickname::initNicknameTexture() {
	settings.antialiasingLevel = 32;

	text.setFont(TextureContainer::getInstance()->getFont());
	text.setCharacterSize(200);
	text.setString(nickname);
	text.setFillColor(sf::Color::White);
	text.setOutlineThickness(10);
	text.setOutlineColor(sf::Color::Black);
	text.setPosition(1920 / 2 - text.getGlobalBounds().width / 2, 0);

	render_texture.create(1920, 1080, settings);
	render_texture.clear(sf::Color(0, 0, 0, 0));
	render_texture.draw(text);
	render_texture.display();
	render_texture.setSmooth(false);
	shape.setTexture(&render_texture.getTexture());
}