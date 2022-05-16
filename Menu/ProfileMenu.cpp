#include "ProfileMenu.h"
#include "../TextureContainer/TextureContainer.h"
#include "../Sounds/SoundContainer.h"
#include "../Game.h"
#include "MainMenu.h"

ProfileMenu::ProfileMenu(sf::RenderWindow* window, Game* game)
	: Menu(window, game), can_play_sounds{ true } {
	buttons.push_back(new Button(400, 100, 200, 300, "Play"));
	buttons[0]->setShapeColor("idle", sf::Color::White);
	buttons[0]->setShapeColor("hover", sf::Color(128, 128, 128, 255));
	buttons[0]->setShapeColor("active", sf::Color(128, 128, 128, 255));
	buttons.push_back(new Button(400, 100, 200, window->getSize().y - 200, "Exit"));
	buttons[1]->setShapeColor("idle", sf::Color::White);
	buttons[1]->setShapeColor("hover", sf::Color(128, 128, 128, 255));
	buttons[1]->setShapeColor("active", sf::Color(128, 128, 128, 255));

	background[0] = new sf::RectangleShape(sf::Vector2f(window->getSize()));
	background[0]->setTexture(TextureContainer::getInstance()->getMainMenuTexture(0));
	background[0]->setPosition(0, 0);
	background[1] = new sf::RectangleShape(sf::Vector2f(window->getSize()));
	background[1]->setTexture(TextureContainer::getInstance()->getMainMenuTexture(0));
	background[1]->setPosition(window->getSize().x, 0);
	title = sf::RectangleShape(sf::Vector2f(700, 400));
	title.setTexture(TextureContainer::getInstance()->getMainMenuTexture(1));
	title.setPosition(buttons[0]->getShape()->getPosition().x + buttons[0]->getShape()->getSize().x / 2 - title.getSize().x / 2,
		0);

	is_camera_switched = false;
	SoundContainer::getInstance()->getMenuMusic()[0]->play();
	SoundContainer::getInstance()->getMenuMusic()[0]->setLoop(true);

	texture_for_anim = TextureContainer::getInstance()->getEntityTextures("Tojic", "Run", "East");
	for_anim = sf::RectangleShape(sf::Vector2f(16 * 2, 32 * 4));
	for_anim.setTexture(texture_for_anim[0]);
	for_anim.setPosition(buttons[0]->getShape()->getPosition().x, buttons[0]->getShape()->getPosition().y - 40);

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