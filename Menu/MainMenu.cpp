#include "MainMenu.h"


MainMenu::MainMenu(sf::RenderWindow* window) : Menu(window) {
	buttons.push_back(new Button(400, 100, 200, 300, "Play"));
	buttons[0]->setShapeColor("idle", sf::Color::White);
	buttons[0]->setShapeColor("hover", sf::Color(128, 128, 128, 255));
	buttons[0]->setShapeColor("active", sf::Color(128, 128, 128, 255));
	buttons.push_back(new Button(400, 100, 200, window->getSize().y - 200, "Exit"));
	buttons[1]->setShapeColor("idle", sf::Color::White);
	buttons[1]->setShapeColor("hover", sf::Color(128, 128, 128, 255));
	buttons[1]->setShapeColor("active", sf::Color(128, 128, 128, 255));
	background = new sf::RectangleShape(sf::Vector2f(window->getSize()));
	background->setFillColor(sf::Color::Black);
	is_camera_switched = false;
}

MainMenu::~MainMenu() {
	delete background;
	for (int i = 0; i < buttons.size(); i++) delete buttons[i];
}

void MainMenu::update(sf::Event& event, Camera* camera) {
	if (camera->getView()->getSize().x == getWindow()->getSize().x && camera->getView()->getSize().y == getWindow()->getSize().y) is_camera_switched = true;
	else is_camera_switched = false;

	if (!is_camera_switched){
		camera->getView()->setSize(sf::Vector2f(getWindow()->getSize()));
		camera->getView()->setCenter(getWindow()->getSize().x / 2, getWindow()->getSize().y / 2);
		getWindow()->setView(*camera->getView());
		is_camera_switched = true;
	}
	sf::Vector2f mouse_pos = getWindow()->mapPixelToCoords(sf::Mouse::getPosition());

	for (int i = 0; i < buttons.size(); i++) buttons[i]->update(mouse_pos, event);

	if (buttons[0]->isClicked()) {
		camera->getView()->setSize(sf::Vector2f(300, 300));
		setActive(false);
	}
	else if (buttons[1]->isClicked()) {
		getWindow()->close();
	}
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates) const {
	target.draw(*background);
	for (int i = 0; i < buttons.size(); i++) target.draw(*buttons[i]);
}