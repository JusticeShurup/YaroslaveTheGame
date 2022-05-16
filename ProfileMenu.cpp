#include "ProfileMenu.h"
#include "../TextureContainer/TextureContainer.h"
#include "../Sounds/SoundContainer.h"
#include "../Game.h"
#include "MainMenu.h"

ProfileMenu::ProfileMenu(sf::RenderWindow* window, Game* game)
	: Menu(window, game), can_play_sounds{ true } {

	is_background_in_pos = false;
	can_close = false;
	is_in_starty = false;
	Menu::setActive(false);

	getGame()->getCamera()->setSize(1920, 1080);
	sf::Vector2f center_view = window->getView().getCenter();
	window->setView(*getGame()->getCamera()->getDefaultView());
	background = sf::RectangleShape(sf::Vector2f(350, 700));
	background.setPosition(center_view.x - background.getSize().x / 2, center_view.y - window->getView().getSize().y / 2 - background.getSize().y);
	background.setOutlineThickness(1);
	background.setOutlineColor(sf::Color::Black);
	background.setFillColor(sf::Color(152, 168, 192, 183));

	unsigned int letters_size = 60;
	pause_text = new sf::Text;
	pause_text->setCharacterSize(letters_size);
	pause_text->setPosition(background.getPosition().x + background.getSize().x / 2 - pause_text->getGlobalBounds().width / 2, background.getPosition().y + 5);
	pause_text->setFont(TextureContainer::getInstance()->getFont());
	pause_text->setOutlineThickness(3);
	pause_text->setOutlineColor(sf::Color::Black);
	pause_text->setString("Pause");

	buttons.push_back(new Button(300, 100, 0, 0, "Resume", letters_size));
	buttons.push_back(new Button(300, 100, 0, 0, "Exit", letters_size));
	sf::Event event;
	for (int i = 0; i < 2; i++) {
		buttons[i]->hideShape(false);
		buttons[i]->setShapeColor("idle", sf::Color(152, 168, 192, 183));
		buttons[i]->setShapeColor("hover", sf::Color(152, 168, 230, 255));
		buttons[i]->setShapeColor("active", sf::Color(152, 168, 230, 255));
		buttons[i]->setOutlineShapeColor("hover", sf::Color::Black);
		buttons[i]->setOutlineShapeColor("active", sf::Color::Black);
		buttons[i]->setTextColor("idle", sf::Color::White);
		buttons[i]->setTextColor("hover", sf::Color(200, 200, 200));
		buttons[i]->setTextColor("active", sf::Color(200, 200, 200));
		buttons[i]->getText()->setOutlineThickness(3);
		buttons[i]->getText()->setOutlineColor(sf::Color::Black);
		buttons[i]->update(sf::Vector2f(0, 0), event);
	}


}

ProfileMenu::~ProfileMenu() {}

void ProfileMenu::setActive(bool flag) {
	if (flag) Menu::setActive(flag);

	if (!flag) {
		can_close = true;
	}
}

void ProfileMenu::update(sf::Event& event, Camera* camera, float delta_time) {
	camera->setSize(1920, 1080);
	sf::Vector2f pos = background.getPosition();
	float targetY = camera->getView()->getCenter().y - background.getSize().y / 2; // coordinate Y where background must be
	float startY = camera->getView()->getCenter().y - background.getSize().y - camera->getView()->getSize().y / 2;

	sf::Vector2f mouse_pos = sf::Vector2f(getWindow()->mapPixelToCoords(sf::Mouse::getPosition()) - sf::Vector2f(0, 24));
	// Блок прокрутки менюшки
	// Вниз
	if (!is_background_in_pos && isActive()) {
		if (!is_in_starty) {
			background.setPosition(camera->getView()->getCenter().x - background.getSize().x / 2, camera->getView()->getCenter().y - background.getSize().y - camera->getView()->getSize().y / 2);
			pos = background.getPosition();
			is_in_starty = true;
		}
		if (pos.y + 1500 * delta_time <= targetY) {
			background.setPosition(camera->getView()->getCenter().x - background.getSize().x / 2, pos.y + 1500 * delta_time);
		}
		else {
			background.setPosition(camera->getView()->getCenter().x - background.getSize().x / 2, targetY);
		}
		if (background.getPosition().y == targetY) {
			is_background_in_pos = true;
		}
	}
	// Вниз 

	// Вверх
	else if (is_background_in_pos && can_close) {
		if (pos.y >= startY) {
			background.setPosition(camera->getView()->getCenter().x - background.getSize().x / 2, pos.y - 1500 * delta_time);
		}
		else {
			background.setPosition(camera->getView()->getCenter().x - background.getSize().x / 2, startY);
		}
		if (background.getPosition().y == startY) {
			is_background_in_pos = false;
			Menu::setActive(false);
			can_close = false;
			is_in_starty = false;
		}
	}
	pause_text->setPosition(background.getPosition().x + background.getSize().x / 2 - pause_text->getGlobalBounds().width / 2, background.getPosition().y);
	// Вверх
	// Блок прокрутки менюшки
	camera->setSize(300, 300);

	buttons[0]->setPosition(pos.x + background.getSize().x / 2 - buttons[0]->getShape()->getSize().x / 2, pos.y + 80);
	buttons[1]->setPosition(pos.x + background.getSize().x / 2 - buttons[0]->getShape()->getSize().x / 2, pos.y + background.getSize().y - 150);


	if (!(!is_background_in_pos && isActive()) && !(is_background_in_pos && can_close)) {
		for (int i = 0; i < buttons.size(); i++) {
			buttons[i]->update(mouse_pos, event);
		}
		for (int i = 0; i < buttons.size(); i++) {
			if (buttons[i]->isClicked()) {
				if (i == 0) {
					setActive(false);
				}
				else if (i == 1) {
					getGame()->setMenu(new MainMenu(getWindow(), getGame()));
					return;
				}
			}
		}
	}


}

void ProfileMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	getGame()->getCamera()->setSize(1920, 1080);
	target.draw(background);
	target.draw(*pause_text);
	for (int i = 0; i < buttons.size(); i++) target.draw(*buttons[i]);
	getGame()->getCamera()->setSize(300, 300);
}