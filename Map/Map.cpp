#include "Map.h"
#include <fstream>
#include <iostream>
#include "../TextureContainer/TextureContainer.h"
#include "../Entity.h"

Map::Map(std::string tilesets_map_filename, std::string gameobjects_map_filename) {
	std::ifstream file(tilesets_map_filename);
	file >> size.x >> size.y >> tileset_size.x >> tileset_size.y;
	for (int i = 0; i < size.y; i++) {
		std::vector<std::string> string;
		std::vector<Tileset> tilesets;
		for (int j = 0; j < size.x; j++) {
			std::string sign;
			file >> sign;
			int sost = sign[sign.size() - 1] - '0';
			sign = sign.substr(0, sign.size() - 2);
			string.push_back(sign);
			tilesets.emplace_back(Tileset(TextureContainer::getInstance()->getTilesetTexture(sign, sost), sign, sf::Vector2f(tileset_size), sf::Vector2f(j*tileset_size.x, i*tileset_size.y), sost));
			tilesets[j].setTempTexture(TextureContainer::getInstance()->getTilesetTexture(sign, sost));
		}
		tileset_signs.push_back(string);
		tileset_map.push_back(tilesets);
	}
	file.close();

	file.open(gameobjects_map_filename);
	while (!file.eof()) {
		std::string gameobj_name;
		float obj_width = 0, obj_height = 0, obj_posx = 0, obj_posy = 0, hitbox_width = 0, hitbox_height = 0, hitbox_posx = 0, hitbox_posy = 0;
		int hitbox_count = 0;
		std::vector<sf::Vector2f> hitbox_sizes;
		std::vector<sf::Vector2f> hitbox_positions;
		file >> gameobj_name >> obj_width >> obj_height >> obj_posx >> obj_posy >> hitbox_count;
		for (int i = 0; i < hitbox_count; i++) {
			file >> hitbox_width >> hitbox_height;
			hitbox_sizes.push_back(sf::Vector2f(hitbox_width, hitbox_height));

			file >> hitbox_posx >> hitbox_posy;
			hitbox_positions.push_back(sf::Vector2f(hitbox_posx, hitbox_posy));
		}
		gameobject_map.push_back(new GameObject(TextureContainer::getInstance()->getGameObjectTexture(gameobj_name), sf::Vector2f(obj_width, obj_height),
			sf::Vector2f(obj_posx, obj_posy), hitbox_count, hitbox_sizes, hitbox_positions, gameobj_name));

	}
	file.close();
	for (auto& g : gameobject_map) {
		g->hideHitboxes(true);
	}
}

void Map::changeTilesetTexture(int x, int y, sf::Texture* texture) {
	//Find Tileset
	Tileset* tileset;
	for (int i = 0; i < tileset_map.size(); i++) {
		for (int j = 0; j < tileset_map[i].size(); j++) {
			tileset = &tileset_map[i][j];
			if (tileset->getShape()->getGlobalBounds().contains(x, y)) break;
		}
	}
	tileset->setTexture(texture);
}

bool Map::isEntity(GameObject* game_object) {
	for (int i = 0; i < entity.size(); i++)
		if (game_object == entity[i])
			return true;
	return false;
}

void Map::addGameObject(GameObject* game_object) {
	for (int i = 0; i < gameobject_map.size(); i++) {
		if (gameobject_map[i] == game_object) {
			return;
		}
	}
	gameobject_map.push_back(game_object);
}

void Map::addEntity(Entity* entity) {
	for (int i = 0; i < this->entity.size(); i++) {
		if (this->entity[i] == entity) {
			return;
		}
	}
	this->entity.push_back(entity);
	addGameObject(entity);
}

void Map::removeGameObject(GameObject* game_object) {
	for (int i = 0; i < gameobject_map.size(); i++) {
		if (gameobject_map[i] == game_object) {
			gameobject_map.erase(gameobject_map.begin() + i);
			break;
		}
	}
	for (int i = 0; i < entity.size(); i++) {
		if (entity[i] == game_object) {
			entity.erase(entity.begin() + i);
			break;
		}
	}
}

void Map::removeGameObjectByIndex(int index) {
	gameobject_map.erase(gameobject_map.begin() + index);
}
	
std::vector<Entity*> Map::getEntityInRange(sf::IntRect range) {
	std::vector<Entity*> entity;
	for (int i = 0; i < this->entity.size(); i++) {
		sf::Vector2f hitbox = this->entity[i]->getHitboxShape()->getSize(); 
		if (range.contains(sf::Vector2i(this->entity[i]->getObjectPosition() + this->entity[i]->getHitboxPosition() + sf::Vector2f(hitbox.x / 2, hitbox.y / 2)))) {
			entity.push_back(this->entity[i]);
		}
	}
	return entity;

}

std::vector<Entity*> Map::getEntity() {
	return entity;
}

GameObject* Map::getGameObjectByCoordinates(sf::Vector2f coordinates) {
	for (int i = 0; i < gameobject_map.size(); i++) {
		if (gameobject_map[i]->getObjectPosition() == coordinates) {
			return gameobject_map[i];
		}
	}
}

GameObject* Map::getGameObjectByIndex(int index) {
	return gameobject_map[index];
}

sf::Vector2i Map::getSize() {
	return size;
}

sf::Vector2i Map::getTilesetSize() {
	return tileset_size;
}

bool Map::checkCollisionWithMap(sf::Vector2f coordinates, GameObject* game_obj) {
	bool flag = false;
	sf::RectangleShape hitboxnewpos(game_obj->getHitboxShape()->getSize());
	hitboxnewpos.setPosition(coordinates);
	for (int i = 0; i < gameobject_map.size(); i++) {
		if (!isEntity(gameobject_map[i])) {
			for (int j = 0; j < gameobject_map[i]->getHitboxShapes()->size(); j++) {
				if (gameobject_map[i] != game_obj) {
					flag = gameobject_map[i]->getHitboxShape(j)->getGlobalBounds().intersects(hitboxnewpos.getGlobalBounds());
				}
				if (flag) return flag;
			}
		}
	}
	return flag;
}

int Map::returnCollisionWithMap(sf::Vector2f coordinates, GameObject* game_obj) {
	bool flag = false;
	sf::RectangleShape hitboxnewpos(game_obj->getHitboxShape()->getSize());
	hitboxnewpos.setPosition(coordinates);
	for (int i = 0; i < gameobject_map.size(); i++) {
		if (!isEntity(game_obj)) {
			for (int j = 0; j < gameobject_map[i]->getHitboxShapes()->size(); j++) {
				if (gameobject_map[i] != game_obj) {
					flag = gameobject_map[i]->getHitboxShape(j)->getGlobalBounds().intersects(hitboxnewpos.getGlobalBounds()) || gameobject_map[i]->getHitboxShape(j)->getGlobalBounds().contains(coordinates);
				}
				if (flag) return i;
			}
		}
	}
	return -1;
}

void Map::update(GameObject* game_obj) {
	for (int i = 0; i < gameobject_map.size(); i++) {
		for (int j = 0; j < gameobject_map.size()-1; j++) {
			if (gameobject_map[j]->getHitboxPosition().y + gameobject_map[j]->getObjectShape()->getPosition().y > gameobject_map[j + 1]->getHitboxPosition().y + gameobject_map[j + 1]->getObjectShape()->getPosition().y) {
				auto temp = gameobject_map[j];
				gameobject_map[j] = gameobject_map[j + 1];
				gameobject_map[j + 1] = temp;
			}
		}
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	for (int i = 0; i < tileset_map.size(); i++) {
		for (int j = 0; j < tileset_map[i].size(); j++) {
			target.draw(tileset_map[i][j]);
		}
	}
	for (int i = 0; i < gameobject_map.size(); i++) {
		target.draw(*gameobject_map[i]);
	}
}