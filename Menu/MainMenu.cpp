#include "MainMenu.h"
#include "../Game.h"
#include "../Sounds/SoundContainer.h"
#include "../TextureContainer/TextureContainer.h"

MainMenu::MainMenu(sf::RenderWindow* window, Game* game) : Menu(window, game) {
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
	is_camera_switched = false;
	SoundContainer::getInstance()->getMenuMusic()[0]->play();
}

MainMenu::~MainMenu() {
	for (int i = 0; i < 2; i++) delete background[i];
	for (int i = 0; i < buttons.size(); i++) delete buttons[i];
	SoundContainer::getInstance()->getMenuMusic()[0]->stop();
}

void MainMenu::update(sf::Event& event, Camera* camera, float delta_time) {
	if (isActive()) {
		if (SoundContainer::getInstance()->getMenuMusic()[0]->getStatus() != sf::Music::Playing) {
			SoundContainer::getInstance()->getMenuMusic()[0]->play();
		}
		if (background[0]->getPosition().x <= -camera->getView()->getSize().x) {
			background[0]->setPosition(background[1]->getPosition().x + background[1]->getSize().x, 0);
		}
		else if (background[1]->getPosition().x <= -camera->getView()->getSize().x) {
			background[1]->setPosition(background[0]->getPosition().x + background[0]->getSize().x, 0);
		}
		background[0]->setPosition(background[0]->getPosition().x - 100 * delta_time, 0);
		background[1]->setPosition(background[1]->getPosition().x - 100 * delta_time, 0);

		if (camera->getView()->getSize().x == getWindow()->getSize().x && camera->getView()->getSize().y == getWindow()->getSize().y) is_camera_switched = true;
		else is_camera_switched = false;

		if (!is_camera_switched) {
			camera->getView()->setSize(sf::Vector2f(getWindow()->getSize()));
			camera->getView()->setCenter(getWindow()->getSize().x / 2, getWindow()->getSize().y / 2);
			getWindow()->setView(*camera->getView());
			is_camera_switched = true;
		}
		sf::Vector2f mouse_pos = getWindow()->mapPixelToCoords(sf::Mouse::getPosition());

		for (int i = 0; i < buttons.size(); i++) buttons[i]->update(mouse_pos, event);

		if (buttons[0]->isClicked()) {
			camera->getView()->setSize(sf::Vector2f(300, 300));
			getWindow()->setView(*camera->getView());
			setActive(false);
			getGame()->setMenu(new PauseMenu(getWindow(), getGame()));
		}
		else if (buttons[1]->isClicked()) {
			getWindow()->close();
			getGame()->setIsRunning(false);
		}
	}
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates) const {
	target.draw(*background[0]);
	target.draw(*background[1]);
	for (int i = 0; i < buttons.size(); i++) target.draw(*buttons[i]);
}