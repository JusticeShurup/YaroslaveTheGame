#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
public: 
	Animation(std::string textures_name, std::string animation_name, std::string direction, float animation_duration);
	Animation(std::string textures_name, std::string animation_name, std::string direction, float animation_duration, bool is_onetime);

	float getAnimationDuration() const;
	sf::Texture* getComponent(int index);
	sf::Texture* getComponent(float elapsed_time);

	bool isOneTime() const;

private:
	
	std::string textures_name;
	std::string animation_name; // animation name means state
	std::string direction;

	float animation_duration;
	bool is_onetime; // Переменная, означающая, что анимацию нужно проиграть лишь раз и затем сменить

	std::vector<sf::Texture*> animation_components;
};
#endif 
