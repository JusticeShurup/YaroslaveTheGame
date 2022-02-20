#ifndef _STATE_H_
#define _STATE_H_
#include <iostream>
#include "SFML/Audio.hpp"

class Entity;
class Map;

#include <SFML/Window.hpp>
class State
{
protected: 
	Entity* entity;
	std::string name;

public: 
	State(Entity* entity);

	std::string getName();

	virtual void update(float delta_time) = 0;
	virtual void update(float delta_time, Entity* target, Map* map) = 0;

	bool operator== (const State &s);

};
#endif
