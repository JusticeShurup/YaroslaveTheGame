#include "Camera.h"

Camera::Camera(sf::Vector2f size, sf::Vector2f position, sf::RenderWindow* window) {
	default_height = size.y;
	default_width = size.x;
	view = sf::View(position, size);
	view_for_render = sf::View(position, size);
	game_window = window;

	camera_locked_x = true;
	camera_locked_y = true;
}

sf::View* Camera::getDefaultView() {
	setSize(300, 300);
	return &view;
}

void Camera::setSize(float x, float y) {
	view.setSize(x, y);
	game_window->setView(view);
}

void Camera::setSize(sf::Vector2f size) {
	view.setSize(size);
	game_window->setView(view);
}

void Camera::setPosition(float x, float y) {
	view.setCenter(x, y);
	game_window->setView(view);
}

void Camera::setPosition(sf::Vector2f position) {
	view.setCenter(position);
	game_window->setView(view);
}

bool Camera::isCameraLockedX() {
	return camera_locked_x;
}

bool Camera::isCameraLockedY() {
	return camera_locked_y;
}

void Camera::update(sf::Vector2f pos, Map* map) {
	if (pos.x - default_width / 2 > 0 && pos.x + default_width / 2 < map->getSize().x * map->getTilesetSize().x) {
		setPosition(pos.x, view.getCenter().y);
		camera_locked_x = false;
	}
	else {
		camera_locked_x = true;
	}
	if (pos.y - default_height / 2 > 0 && pos.y + default_height / 2 < map->getSize().y * map->getTilesetSize().y) {
		setPosition(view.getCenter().x, pos.y);
		camera_locked_y = false;
	}
	else {
		camera_locked_y = true;
	}
	view_for_render.setCenter(pos);
}

sf::Transform Camera::getTransform(float x, float y, int width, int height) {
	sf::View new_view(sf::Vector2f(x, y), sf::Vector2f(width, height));
	return new_view.getTransform();
}

sf::View* Camera::getView() {
	return &view;
}

sf::View* Camera::getRenderView() {
	return &view_for_render;
}

sf::RenderWindow* Camera::getWindow() const {
	return game_window;
}