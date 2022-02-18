#include "Camera.h"

Camera::Camera(sf::Vector2f size, sf::Vector2f position) {
	view = sf::View(position, size);
}

void Camera::setSize(float x, float y) {
	view.setSize(x, y);
}

void Camera::setSize(sf::Vector2f size) {
	view.setSize(size);
}

void Camera::setPosition(float x, float y) {
	view.setCenter(x, y);
}

void Camera::setPosition(sf::Vector2f position) {
	view.setCenter(position);
}

void Camera::update(sf::Vector2f pos, Map* map) {
	if (pos.x - view.getSize().x / 2 > 0 && pos.x + view.getSize().x / 2 < map->getSize().x * map->getTilesetSize().x) {
		setPosition(pos.x, view.getCenter().y);
	}
	if (pos.y - view.getSize().y / 2 > 0 && pos.y + view.getSize().y / 2 < map->getSize().y * map->getTilesetSize().y) {
		setPosition(view.getCenter().x, pos.y);
	}


}

sf::View* Camera::getView() {
	return &view;
}