#include "Menu.h"

Menu::Menu(sf::RenderWindow* window) {
	this->window = window;
	is_active = true;
}

void Menu::setActive(bool flag) {
	is_active = flag;
}

bool Menu::isActive() {
	return is_active;
}

sf::RenderWindow* Menu::getWindow() {
	return window;
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {

}