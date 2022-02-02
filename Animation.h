#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
public: 
	Animation(std::string textures_name, std::string animation_name, std::string direction, float animation_duration);

	float getAnimationDuration() const;
	sf::Texture* getComponent(int index);
	sf::Texture* getComponent(float elapsed_time);

private:
	
	std::string textures_name;
	std::string animation_name; // animation name means state
	std::string direction;

	float animation_duration;

	std::vector<sf::Texture*> animation_components;
};
#endif 
