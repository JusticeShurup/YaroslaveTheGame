#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_
#include "Animation.h"
class Entity;

class Animator
{
public: 
	Animator(Entity* entity, std::string textures_name);

	~Animator();

	void setTexturesName(std::string textures_name);
	std::string getTexturesName();

	void update(float delta_time);
private: 
	Entity* entity;

	std::string last_state;

	float elapsed_time; // Elapsed time since start
	std::string textures_name;

	std::map<std::string, std::map<std::string, Animation*>> animations; // Sost for animations;
};
#endif