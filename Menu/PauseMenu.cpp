#include "PauseMenu.h"
#include <iostream>
#include "../TextureContainer/TextureContainer.h"
#include "../Sounds/SoundContainer.h"
#include "../Game.h"
#include "MainMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow* window, Game* game) 
	: Menu(window, game), can_play_sounds{true} {
	is_background_in_pos = false;
	setActive(false);

	sf::Vector2f center_view = window->getView().getCenter();
	background = sf::RectangleShape(sf::Vector2f(50, 100));
	background.setPosition(center_view.x - background.getSize().x / 2, center_view.y - window->getView().getSize().y / 2 - background.getSize().y);
	background.setOutlineThickness(1);
	background.setOutlineColor(sf::Color::Black);
	background.setTexture(TextureContainer::getInstance()->getPauseMenuTexture(0));


	buttons.push_back(new sf::RectangleShape(sf::Vector2f(40, 20)));
	buttons[0]->setTexture(TextureContainer::getInstance()->getPauseMenuTexture(1), 1);
	buttons[0]->setOutlineThickness(1);
	buttons[0]->setOutlineColor(sf::Color::Black);

	buttons.push_back(new sf::RectangleShape(sf::Vector2f(40, 20)));
	buttons[1]->setTexture(TextureContainer::getInstance()->getPauseMenuTexture(3), 1);
	buttons[1]->setOutlineThickness(1);
	buttons[1]->setOutlineColor(sf::Color::Black);

}

PauseMenu::~PauseMenu(){}

void PauseMenu::setActive(bool flag) {
	Menu::setActive(flag);

	if (flag) {
		background.setPosition(getWindow()->getView().getCenter().x - background.getSize().x / 2, getWindow()->getView().getCenter().y - getWindow()->getView().getSize().y / 2 - background.getSize().y);
		is_background_in_pos = false;
	}
}

void PauseMenu::update(sf::Event& event, Camera* camera, float delta_time) {
	sf::Vector2f pos = background.getPosition();
	float targetY = camera->getView()->getCenter().y - background.getSize().y / 2; // coordinate Y where background must be

	sf::Vector2f mouse_pos = getWindow()->mapPixelToCoords(sf::Mouse::getPosition());

	for (int i = 1; i <= buttons.size(); i++) {
		if (buttons[i-1]->getGlobalBounds().contains(mouse_pos.x - 1, mouse_pos.y - 7)) {
			buttons[i-1]->setTexture(TextureContainer::getInstance()->getPauseMenuTexture(i * 2));
			if (can_play_sounds[i-1]) {
				SoundContainer::getInstance()->getSoundByName("Hover")->play();
				can_play_sounds[i - 1] = false;
			}
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				if (i - 1 == 0) {
					setActive(false);
				}
				else if (i - 1 == 1) {
					getGame()->setMenu(new MainMenu(getWindow(), getGame()));
					return;
				}
			}
		}
		else{
			buttons[i - 1]->setTexture(TextureContainer::getInstance()->getPauseMenuTexture(i * 2 - 1));
			can_play_sounds[i - 1] = true;
		}
	}



	if (!is_background_in_pos && isActive()) {
		if (pos.y + 500 * delta_time <= targetY) {
			background.setPosition(camera->getView()->getCenter().x - background.getSize().x / 2, pos.y + 500 * delta_time);
		}
		else {
			background.setPosition(camera->getView()->getCenter().x - background.getSize().x / 2, targetY);
		}
		if (background.getPosition().y == camera->getView()->getCenter().y - background.getSize().y / 2) {
			is_background_in_pos = true;
		}
	}

	buttons[0]->setPosition(pos.x + 5, pos.y + 20);
	buttons[1]->setPosition(pos.x + 5, pos.y + 70);
}

void PauseMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(background);
	for (int i = 0; i < buttons.size(); i++) target.draw(*buttons[i]);
}

