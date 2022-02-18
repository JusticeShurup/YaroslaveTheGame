#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <SFML/Graphics.hpp>
#include "Map/Map.h"
class Camera
{
public: 
	Camera(sf::Vector2f size, sf::Vector2f position);

	void setSize(float x, float y);
	void setSize(sf::Vector2f position);

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position); 
	
	void update(sf::Vector2f position, Map* map);

	sf::View* getView();
private: 
	sf::View view;
};
#endif 
