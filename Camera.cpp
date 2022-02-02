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

sf::View* Camera::getView() {
	return &view;
}