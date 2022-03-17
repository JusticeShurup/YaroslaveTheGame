#include "TextureContainer.h"
#include <string>
#include <iostream>

TextureContainer* TextureContainer::instance = nullptr;

TextureContainer::TextureContainer() {
	std::ifstream file("TextureContainer\\Tilesets\\TilesetTextureAlphabet.txt");
	std::string sign;
	std::string filename;
	while (!file.eof()) {
		file >> sign >> filename;
		int tilesets_size = std::atoi(filename.c_str());
		std::string category_name = sign;
		
		std::map<std::string, std::map<uint8_t, sf::Texture*>> alphabet_textures_for_pallete;
		for (int i = 0; i < tilesets_size; i++) {
			file >> sign >> filename;

			sf::Image* image = new sf::Image;
			sf::Texture* texture = new sf::Texture;

			image->loadFromFile(filename);
			texture->loadFromFile(filename);

			alphabet_tilesets_images.emplace(sign, image);

			std::map<uint8_t, sf::Texture*> map;
			map[1] = texture;
			alphabet_tilesets_textures.emplace(sign, map);
			alphabet_textures_for_pallete.emplace(sign, map);

			for (int i = 2; i <= 4; i++) {
				alphabet_tilesets_textures[sign].emplace(i, rotateTexture(alphabet_tilesets_textures[sign][i - 1]));
				alphabet_textures_for_pallete[sign].emplace(i, rotateTexture(alphabet_textures_for_pallete[sign][i - 1]));
			}

			alphabet_tilesets_links.emplace(sign, filename);
			tilesets_signs.push_back(sign);
			tilesets_textures.push_back(texture);
			tilesets_links.push_back(filename);
		}
		alphabet_textures_tilesets_pallete.emplace(category_name, alphabet_textures_for_pallete);
	}
	file.close();
	file.open("TextureContainer\\GameObjects\\GameObjectTextureAlphabet.txt");
	while (!file.eof()) {
		file >> sign >> filename;
		int gameobjects_size = std::atoi(filename.c_str());
		std::string category_name = sign;

		std::map<std::string, sf::Texture*> alphabet_texture_for_pallete;
		for (int i = 0; i < gameobjects_size; i++) {
			file >> sign >> filename;
			float width = 0;
			float heigth = 0;
			int hitbox_count = 0;
			file >> width >> heigth;

			gameobjects_native_size.emplace(sign, sf::Vector2f(width, heigth));

			file >> hitbox_count;
			std::vector<sf::Vector2f> hitbox_sizes;
			std::vector<sf::Vector2f> hitbox_positions;

			for (int i = 0; i < hitbox_count; i++) {
				file >> width >> heigth;
				hitbox_sizes.emplace_back(sf::Vector2f(width, heigth));

				file >> width >> heigth;
				hitbox_positions.emplace_back(sf::Vector2f(width, heigth));
			}

			gameobjects_hitbox_size.emplace(sign, hitbox_sizes);
			gameobjects_hitbox_position.emplace(sign, hitbox_positions);

			sf::Image* image = new sf::Image;
			sf::Texture* texture = new sf::Texture;

			image->loadFromFile(filename);
			texture->loadFromFile(filename);

			alphabet_gameobjects_images.emplace(sign, image);
			alphabet_gameobjects_textures.emplace(sign, texture);
			alphabet_texture_for_pallete.emplace(sign, texture);

			alphabet_gameobjects_links.emplace(sign, filename);
			gameobjects_names.push_back(sign);
			gameobjects_textures.push_back(texture);
			gameobjects_links.push_back(filename);
		}
		alphabet_textures_gameobjects_pallete.emplace(category_name, alphabet_texture_for_pallete);
	}
	file.close();

	font.loadFromFile("TextureContainer\\Fonts\\Roboto-Bold.ttf");

	//Textures for Entity
	file.open("TextureContainer\\Entity\\EntityTexturesAlphabet.txt");
	while (!file.eof()) {
		std::string textures_name;
		std::string path;
		int sost_count;
		file >> textures_name >> path >> sost_count;
		std::map<std::string, std::map<std::string, std::vector<sf::Texture*>>> states;
		for (int i = 0; i < sost_count; i++) {
			std::string state;
			int directions_count;
			file >> state >> directions_count;
			std::map<std::string, std::vector<sf::Texture*>> directions;
			for (int j = 0; j < directions_count; j++) {
				std::string direction;
				int textures_count;
				file >> direction >> textures_count;
				std::vector<sf::Texture*> direction_textures;
				for (int z = 0; z < textures_count; z++) {
					sf::Texture* texture = new sf::Texture;
					std::string filename;
					file >> filename;
					texture->loadFromFile("TextureContainer\\Entity\\" + path + "\\" + state + "\\" + filename);
					direction_textures.emplace_back(texture);
				}
				directions.emplace(direction, direction_textures);
			}
			states.emplace(state, directions);
		}
		entity_textures.emplace(textures_name, states);
	}
	file.close();
	//Textures for Entity

	//Textures for Menu
	file.open("TextureContainer\\Menu\\MenuTexturesAlphabet.txt");
	while (!file.eof()) {
		std::string cathegory; 
		int size;
		file >> cathegory >> size;
		std::vector<sf::Texture*> textures;
		for (int i = 0; i < size; i++) {
			std::string filename;
			file >> filename;
			sf::Texture* texture = new sf::Texture;
			texture->loadFromFile(filename);
			textures.push_back(texture);
		}
		menu_textures.emplace(cathegory, textures);
	}
	file.close();
	//Textures for Menu
}



//tilesets
sf::Texture* TextureContainer::getTilesetTexture(std::string sign, uint8_t sost) {
	if (sost < 1 || sost > 4) sost = 1;
	return alphabet_tilesets_textures[sign][sost];
}

std::string TextureContainer::getTilesetLink(std::string sign) {
	return alphabet_tilesets_links[sign];
}

std::vector<sf::Texture*> TextureContainer::getTilesetsTextures() {
	return tilesets_textures;
}

std::map<std::string, std::map<uint8_t, sf::Texture*>> TextureContainer::getTilesetsTexturesByCathegory(std::string cathegory_name){
	return alphabet_textures_tilesets_pallete[cathegory_name];
}

std::vector<std::string> TextureContainer::getTilesetsSigns() {
	return tilesets_signs;
}

std::vector<std::string> TextureContainer::getTilesetsLinks() {
	return tilesets_links;
}
//Tilesets

//GameObjects
sf::Texture* TextureContainer::getGameObjectTexture(std::string name) {
	return alphabet_gameobjects_textures[name];
}

std::string TextureContainer::getGameObjectLink(std::string name) {
	return alphabet_gameobjects_links[name];
}

std::vector<sf::Texture*> TextureContainer::getGameObjectTextures() {
	return gameobjects_textures;
}

std::map<std::string, sf::Texture*> TextureContainer::getGameObjectsTexturesByCathegory(std::string cathegory_name) {
	return alphabet_textures_gameobjects_pallete[cathegory_name];
}

std::vector<std::string> TextureContainer::getGameObjectsNames() {
	return gameobjects_names;
}

std::vector<std::string> TextureContainer::getGameObjectsLinks() {
	return gameobjects_links;
}

sf::Vector2f TextureContainer::getGameObjectNativeSize(std::string name) {
	return gameobjects_native_size[name];
}

std::vector<sf::Vector2f> TextureContainer::getGameObjectHitboxSize(std::string name) {
	return gameobjects_hitbox_size[name];
}

std::vector<sf::Vector2f> TextureContainer::getGameObjectHitboxPosition(std::string name) {
	return gameobjects_hitbox_position[name];
}

//GameObjects

sf::Texture* TextureContainer::rotateTexture(sf::Texture* texture) { 
	sf::Image image = texture->copyToImage();
	const sf::Uint8* pixels = image.getPixelsPtr();

	std::vector<std::vector<sf::Color>> texture_in_pixels;
	sf::Uint8* pix = new sf::Uint8[32 * 32 * 4];

	int count = 0;
	for (int i = 0; i < 32 * 32 * 4; i += 4) {
		pix[i] = pixels[(32 * 32 * 4 - 1) - (i + 3)];
		pix[i + 1] = pixels[(32 * 32 * 4 - 1) - (i + 2)];
		pix[i + 2] = pixels[(32 * 32 * 4 - 1) - (i + 1)];
		pix[i + 3] = 255;
		count++;
	}

	int y = 0;
	int x = 0;
	std::vector<sf::Color> matrix;
	for (int i = 0; i < 32 * 32 * 4; i += 4) {
		matrix.push_back(sf::Color(pix[i], pix[i + 1], pix[i + 2], pix[i + 3]));
		x++;
		if (x == 32) {
			x = 0;
			y++;
			texture_in_pixels.push_back(matrix);
			matrix.clear();
		}
	}

	int n = 32;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			sf::Color temp = texture_in_pixels[i][j];
			texture_in_pixels[i][j] = texture_in_pixels[j][i];
			texture_in_pixels[j][i] = temp;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n / 2; j++) {
			sf::Color temp = texture_in_pixels[i][n - j - 1];
			texture_in_pixels[i][n - j - 1] = texture_in_pixels[i][j];
			texture_in_pixels[i][j] = temp;
		}
	}

	y = 0;
	x = 0;
	for (int i = 0; i < 32 * 32 * 4; i += 4) {
		pix[i] = texture_in_pixels[y][x].r;
		pix[i + 1] = texture_in_pixels[y][x].g;
		pix[i + 2] = texture_in_pixels[y][x].b;
		pix[i + 3] = texture_in_pixels[y][x].a;
		x++;
		if (x == 32) {
			x = 0;
			y++;
		}
	}

	sf::Texture* texture_new = new sf::Texture;
	texture_new->create(32, 32);
	texture_new->update(pix);

	return texture_new;

	delete[] pix;
}

//Fonts
sf::Font& TextureContainer::getFont() {
	return font;
}
//Fonts

//Entity
sf::Texture* TextureContainer::getEntityTexture(std::string name, std::string state, std::string direction, int frame) {
	return entity_textures[name][state][direction][frame];
}

std::vector<sf::Texture*> TextureContainer::getEntityTextures(std::string name, std::string state, std::string direction) {
	return entity_textures[name][state][direction];
}

int TextureContainer::getAnimationSize(std::string name, std::string state, std::string direction) {
	return entity_textures[name][state][direction].size();
}
//Entity

//Menu

std::vector<sf::Texture*> TextureContainer::getMainMenuTextures() {
	return menu_textures["MainMenu"];
}

std::vector<sf::Texture*> TextureContainer::getPauseMenuTextures() {
	return menu_textures["PauseMenu"];
}

sf::Texture* TextureContainer::getMainMenuTexture(int number) {
	if (number < 0 || number >= menu_textures["MainMenu"].size()) {
		number = 0;
	}

	return menu_textures["MainMenu"][number];
}

sf::Texture* TextureContainer::getPauseMenuTexture(int number) {
	if (number < 0 || number >= menu_textures["PauseMenu"].size()) {
		number = 0;
	}

	return menu_textures["PauseMenu"][number];
}

//Menu

TextureContainer* TextureContainer::getInstance() {
	if (!TextureContainer::instance) {
		TextureContainer::instance = new TextureContainer;
	}
	return TextureContainer::instance;
}