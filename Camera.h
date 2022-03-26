#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <SFML/Graphics.hpp>
#include "Map/Map.h"
class Camera
{
public: 
	Camera(sf::Vector2f size, sf::Vector2f position, sf::RenderWindow* game_window);

	// Change to default view and return new view 
	
	void setSize(float x, float y);
	void setSize(sf::Vector2f position);

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position); 
	

	bool isCameraLockedY();
	bool isCameraLockedX();
	void update(sf::Vector2f position, Map* map);

	sf::Transform getTransform(float x, float y, int width, int height);

	sf::View* getView();
	sf::View* getDefaultView();
	//Return view, that hasn't limitation
	sf::View* getRenderView(); 

	sf::RenderWindow* getWindow() const;
private: 
	int default_height;
	int default_width;

	bool camera_locked_x;
	bool camera_locked_y;

	sf::View view;
	sf::View view_for_render;
	sf::RenderWindow* game_window;
};
#endif 
