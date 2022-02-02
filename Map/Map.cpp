#include "Map.h"
#include <fstream>
#include <iostream>
#include "../TextureContainer/TextureContainer.h"

Map::Map(std::string tilesets_map_filename, std::string gameobjects_map_filename) {
	std::ifstream file(tilesets_map_filename);
	file >> size.x >> size.y;
	for (int i = 0; i < size.x; i++) {
		std::vector<std::string> string;
		std::vector<Tileset> tilesets;
		for (int j = 0; j < size.y; j++) {
			std::string sign;
			file >> sign;
			int sost = sign[sign.size() - 1] - '0';
			sign = sign.substr(0, sign.size() - 2);
			string.push_back(sign);
			tilesets.emplace_back(Tileset(TextureContainer::getInstance()->getTilesetTexture(sign, sost), sign, sf::Vector2f(32, 32), sf::Vector2f(j*32, i*32), sost));
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
		gameobject_map.push_back(GameObject(TextureContainer::getInstance()->getGameObjectTexture(gameobj_name), sf::Vector2f(obj_width, obj_height),
			sf::Vector2f(obj_posx, obj_posy), hitbox_count, hitbox_sizes, hitbox_positions, gameobj_name));

	}
	file.close();
	for (auto& g : gameobject_map) {
		g.hideHitboxes(false);
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

void Map::addGameObject(GameObject* game_object) {
	gameobject_map.push_back(*game_object);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	for (int i = 0; i < tileset_map.size(); i++) {
		for (int j = 0; j < tileset_map[i].size(); j++) {
			target.draw(tileset_map[i][j]);
		}
	}
	for (int i = 0; i < gameobject_map.size(); i++) {
		target.draw(gameobject_map[i]);
	}
}