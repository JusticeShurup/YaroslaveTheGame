#include "Button.h"
#include "TextureContainer/TextureContainer.h"
#include "Sounds/SoundContainer.h"
#include <iostream>

using namespace sf;

Button::Button() {}

Button::Button(float widht, float height, float posX, float posY, std::string name)
	: widht(widht), height(height), posX(posX), posY(posY), buttonText(name)
{

	textIdleColor = sf::Color::Black;

	state = BTN_IDLE;
	shape = RectangleShape(Vector2f(widht, height));
	shape.setPosition(Vector2f(posX, posY));
	shape.setOutlineThickness(5);

	text.setFont(TextureContainer::getInstance()->getFont());
	text.setCharacterSize(50);
	text.setFillColor(Color::Black);
	text.setString(name);
	text.setPosition(shape.getPosition().x + ((shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
		shape.getPosition().y);

	canPlaySound = true;
	shape_is_hide = false;
}

Button::Button(float widht, float height, float posX, float posY, std::string name, bool hide_shape)
	: widht(widht), height(height), posX(posX), posY(posY), buttonText(name)
{

	textIdleColor = sf::Color::Black;

	state = BTN_IDLE;
	shape = RectangleShape(Vector2f(widht, height));
	shape.setPosition(Vector2f(posX, posY));
	shape.setOutlineThickness(5);

	text.setFont(TextureContainer::getInstance()->getFont());
	text.setCharacterSize(50);
	text.setFillColor(Color::Black);
	text.setString(name);
	text.setPosition(shape.getPosition().x + ((shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
		shape.getPosition().y);

	buffer.loadFromFile("Sounds/menu_item_howered.wav");
	sound.setBuffer(buffer);
	canPlaySound = true;
	shape_is_hide = hide_shape;
}

Button::Button(float widht, float height, float posX, float posY, std::string name, unsigned int letters_size)
	: widht(widht), height(height), posX(posX), posY(posY), buttonText(name)
{

	state = BTN_IDLE;
	shape = RectangleShape(Vector2f(widht, height));
	shape.setPosition(Vector2f(posX, posY));
	shape.setOutlineThickness(5);

	text.setFont(TextureContainer::getInstance()->getFont());
	text.setCharacterSize(letters_size);
	text.setFillColor(Color::Black);
	text.setString(name);
	text.setPosition(shape.getPosition().x + ((shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
		shape.getPosition().y);
}

bool Button::isClicked() {
	if (state == BTN_ACTIVE) {
		return true;
	}
	else return false;
}

void Button::setPosition(float x, float y) {
	shape.setPosition(x, y);
	text.setPosition(x + (shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2), y);

}

void Button::setPosition(sf::Vector2f position) {
	shape.setPosition(position);
	text.setPosition(position.x + ((shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
		position.y);
}

void Button::setTextColor(std::string sost, sf::Color color) {
	if (sost == "idle") textIdleColor = color;
	else if (sost == "hover") textHoverColor = color;
	else if (sost == "active") textActiveColor = color;
}

void Button::setShapeColor(std::string sost, sf::Color color) {
	if (sost == "idle") shapeIdleColor = color;
	else if (sost == "hover") shapeHoverColor = color;
	else if (sost == "active") shapeActiveColor = color;
}

sf::RectangleShape* Button::getShape() {
	return &shape;
}

sf::Text* Button::getText() {
	return &text;
}

void Button::update(Vector2f pos, Event& event) {
	state = BTN_IDLE;
	//Hover
	if (shape.getGlobalBounds().contains(pos) || text.getGlobalBounds().contains(pos))
	{
		state = BTN_HOVER;

		if (canPlaySound) SoundContainer::getInstance()->getSoundByName("Hover")->play();
		canPlaySound = false;
		//Pressed
		if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left)
		{
			state = BTN_ACTIVE;
			if(text.getString() != "Exit") SoundContainer::getInstance()->getSoundByName("Active")->play();
		}
	}
	else {
		canPlaySound = true;
	}

	switch (state)
	{
	case BTN_IDLE:
		this->shape.setFillColor(shapeIdleColor);
		this->shape.setOutlineThickness(5);
		this->text.setFillColor(textIdleColor);
		this->shape.setOutlineColor(Color::Black);
		break;

	case BTN_HOVER:
		this->shape.setFillColor(shapeHoverColor);
		this->shape.setOutlineThickness(5);
		this->text.setFillColor(textHoverColor);
		this->shape.setOutlineColor(Color(127, 127, 127));
		break;

	case BTN_ACTIVE:
		this->shape.setFillColor(shapeActiveColor);
		this->text.setFillColor(textActiveColor);
		this->shape.setOutlineColor(Color(127, 127, 127));
		break;

	default:
		this->shape.setFillColor(Color::White);
		this->text.setFillColor(textIdleColor);
		this->shape.setOutlineColor(Color::White);
		break;
	}
}

void Button::draw(RenderTarget& target, RenderStates states) const {
	if (!shape_is_hide) target.draw(shape);
	target.draw(text);
}