#ifndef _TEXTURE_CONTAINER_H_
#define _TEXTURE_CONTAINER_H_
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>

class TextureContainer
{
public:
	static TextureContainer* getInstance();

	sf::Image* getImage(std::string sign);

	// Tilesets
	
	sf::Texture* getTilesetTexture(std::string sign, uint8_t sost);
	std::string getTilesetLink(std::string sign);
	std::vector<sf::Texture*> getTilesetsTextures();
	std::map<std::string, std::map<uint8_t, sf::Texture*>> getTilesetsTexturesByCathegory(std::string category_name);
	std::vector<std::string> getTilesetsSigns();
	std::vector<std::string> getTilesetsLinks();

	//Tilesets

	//GameObjects

	sf::Texture* getGameObjectTexture(std::string name);
	std::string getGameObjectLink(std::string name);
	std::vector<sf::Texture*> getGameObjectTextures();
	std::map<std::string, sf::Texture*> getGameObjectsTexturesByCathegory(std::string cathegory_name);
	std::vector<std::string> getGameObjectsNames();
	std::vector<std::string> getGameObjectsLinks();
	sf::Vector2f getGameObjectNativeSize(std::string name);
	std::vector<sf::Vector2f> getGameObjectHitboxSize(std::string name);
	std::vector<sf::Vector2f> getGameObjectHitboxPosition(std::string name);

	//GameObjects

	sf::Texture* rotateTexture(sf::Texture* texture);

	//Fonts
	sf::Font& getFont();
	//Fonts

	//Entity
	sf::Texture* getEntityTexture(std::string textures_name, std::string state, std::string direction, int frame);// Textures_name -> State -> Direction -> Frame
	std::vector<sf::Texture*> getEntityTextures(std::string textures_name, std::string state, std::string direction); // Textures_name -> State -> Direction -> Frames
	int getAnimationSize(std::string textures_name, std::string state, std::string direction); // Return quantity of textures in direction 
	//Entity

	//Menu
	
	std::vector<sf::Texture*> getMainMenuTextures();
	std::vector<sf::Texture*> getPauseMenuTextures();

	sf::Texture* getMainMenuTexture(int number);
	sf::Texture* getPauseMenuTexture(int number);

	//Menu


private: 
	TextureContainer();

	//Tilesets

	std::map<std::string, sf::Image*> alphabet_tilesets_images;
	std::map<std::string, std::map<std::string, std::map<uint8_t, sf::Texture*>>> alphabet_textures_tilesets_pallete;
	std::map<std::string, std::map<uint8_t, sf::Texture*>> alphabet_tilesets_textures;
	std::map<std::string, std::string> alphabet_tilesets_links;
	std::vector<sf::Image*> tilesets_images;
	std::vector<std::string> tilesets_signs;
	std::vector<sf::Texture*> tilesets_textures;
	std::vector<std::string> tilesets_links;
	
	//Tilesets

	//GameObjects
	
	std::map<std::string, sf::Image*> alphabet_gameobjects_images;
	std::map<std::string, std::map<std::string, sf::Texture*>> alphabet_textures_gameobjects_pallete;
	std::map<std::string, sf::Texture*> alphabet_gameobjects_textures; 
	std::map<std::string, std::string> alphabet_gameobjects_links;
	std::vector<sf::Image*> gameobjects_images;
	std::vector<std::string> gameobjects_names;
	std::vector<sf::Texture*> gameobjects_textures;
	std::vector<std::string> gameobjects_links;
	std::map<std::string, sf::Vector2f> gameobjects_native_size;
	std::map<std::string, std::vector<sf::Vector2f>> gameobjects_hitbox_size;
	std::map<std::string, std::vector<sf::Vector2f>> gameobjects_hitbox_position;
	
	//GameObjects

	//Fonts
	sf::Font font;
	//Fonts

	//Entity
	std::map<std::string, std::map<std::string, std::map<std::string, std::vector<sf::Texture*>>>> entity_textures; // Textures_name -> State -> Direction -> Frame
	//Entity

	//Menu
	std::map<std::string, std::vector<sf::Texture*>> menu_textures;
	//Menu

	static TextureContainer* instance;
};
#endif