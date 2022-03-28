#include "Button.h"
#include "TextureContainer/TextureContainer.h"
#include "Sounds/SoundContainer.h"
#include <iostream>

using namespace sf;

Button::Button() {}

Button::Button(float widht, float height, float posX, float posY, std::string name)
	: buttonText(name)
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
	text_dx = 0; 
	text_dy = 0;
	text.setPosition(shape.getPosition().x + text_dx + ((shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
				     shape.getPosition().y + text_dy + ((shape.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 2)) / 2);

	can_play_hover_sound = true;
	can_play_click_sound = true;
	shape_is_hide = false;
}

Button::Button(float widht, float height, float posX, float posY, std::string name, bool hide_shape)
	: buttonText(name)
{
	textIdleColor = sf::Color::Black;
	textHoverColor = sf::Color::White;
	textActiveColor = sf::Color::White;

	shapeIdleColor = sf::Color::White;
	shapeHoverColor = sf::Color(127, 127, 127, 255);
	shapeActiveColor = sf::Color(127, 127, 127, 255);
	
	shapeOutlineIdleColor = sf::Color::Black;
	shapeOutlineHoverColor = sf::Color(127, 127, 127, 255);
	shapeOutlineActiveColor = sf::Color(127, 127, 127, 255);
	


	state = BTN_IDLE;
	shape = RectangleShape(Vector2f(widht, height));
	shape.setPosition(Vector2f(posX, posY));
	shape.setOutlineThickness(5);

	text.setFont(TextureContainer::getInstance()->getFont());
	text.setCharacterSize(50);
	text.setFillColor(Color::Black);
	text.setString(name);
	text_dx = 0;
	text_dy = 0;
	text.setPosition(shape.getPosition().x + text_dx +((shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
					 shape.getPosition().y + text_dy + ((shape.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 2)) / 2);

	can_play_hover_sound = true;
	can_play_click_sound = true;
	shape_is_hide = hide_shape;
}

Button::Button(float widht, float height, float posX, float posY, std::string name, unsigned int letters_size)
	: buttonText(name)
{
	textIdleColor = sf::Color::Black;
	textHoverColor = sf::Color::White;
	textActiveColor = sf::Color::White;

	shapeIdleColor = sf::Color::White;
	shapeHoverColor = sf::Color(127, 127, 127, 255);
	shapeActiveColor = sf::Color(127, 127, 127, 255);

	shapeOutlineIdleColor = sf::Color::Black;
	shapeOutlineHoverColor = sf::Color(127, 127, 127, 255);
	shapeOutlineActiveColor = sf::Color(127, 127, 127, 255);


	state = BTN_IDLE;
	shape = RectangleShape(Vector2f(widht, height));
	shape.setPosition(Vector2f(posX, posY));
	shape.setOutlineThickness(5);

	text.setFont(TextureContainer::getInstance()->getFont());
	text.setCharacterSize(letters_size);
	text.setFillColor(Color::Black);
	text.setString(name);
	text_dx = 0;
	text_dy = 0;
	text.setPosition(shape.getPosition().x + ((shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
					 shape.getPosition().y + text.getGlobalBounds().height / 2);
	

	can_play_hover_sound = true;
	can_play_click_sound = true;
}

bool Button::isClicked() {
	if (state == BTN_ACTIVE) {
		return true;
	}
	else return false;
}

void Button::hideShape(bool flag) {
	shape_is_hide = flag;
}

bool Button::isShapeHidden() const {
	return shape_is_hide;
}

void Button::setPosition(float x, float y) {
	shape.setPosition(x, y);
	text.setPosition(x + text_dx +(shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2),
					 y + text_dy + ((shape.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 2)) / 2);

}

void Button::setPosition(sf::Vector2f position) {
	shape.setPosition(position);
	text.setPosition(position.x + text_dx + ((shape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
					 position.y + text_dy + ((shape.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 2)) / 2);
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

void Button::setOutlineShapeColor(std::string sost, sf::Color color) {
	if (sost == "idle") shapeOutlineIdleColor = color;
	else if (sost == "hover") shapeOutlineHoverColor = color;
	else if (sost == "active") shapeOutlineActiveColor = color;
}

void Button::setTextDX(float value) {
	text_dx = value;
}

float Button::getTextDX() {
	return text_dx;
}

void Button::setTextDY(float value) {
	text_dy = value;
}

float Button::getTextDY() {
	return text_dy;
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

		if (can_play_hover_sound) SoundContainer::getInstance()->getSoundByName("Hover")->play();
		can_play_hover_sound = false;
		//Pressed
		if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left)
		{
			state = BTN_ACTIVE;
			if(can_play_click_sound) SoundContainer::getInstance()->getSoundByName("Active")->play();
			can_play_click_sound = false;
			event.type = event.MouseMoved;
		}
		else {
			can_play_click_sound = true;
		}
	}
	else {
		can_play_hover_sound = true;
	}

	switch (state)
	{
	case BTN_IDLE:
		this->shape.setFillColor(shapeIdleColor);
		this->shape.setOutlineThickness(3);
		this->text.setFillColor(textIdleColor);
		this->shape.setOutlineColor(Color::Black);
		break;

	case BTN_HOVER:
		this->shape.setFillColor(shapeHoverColor);
		this->shape.setOutlineThickness(3);
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