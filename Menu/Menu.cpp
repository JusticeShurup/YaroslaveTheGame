#include "Menu.h"
#include "../Game.h"

Menu::Menu(sf::RenderWindow* window, Game* game) {
	this->window = window;
	this->game = game;
	is_active = true;
}

void Menu::setActive(bool flag) {
	is_active = flag;
}

bool Menu::isActive() {
	return is_active;
}

Game* Menu::getGame() const {
	return game;
}

sf::RenderWindow* Menu::getWindow() {
	return window;
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {

}