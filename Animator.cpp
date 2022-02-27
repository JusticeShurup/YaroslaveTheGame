#include "Animator.h"
#include "TextureContainer/TextureContainer.h"
#include "Entity.h"
#include "States/IdleState.h"
#include <iostream>

std::string states[5]{ "Idle", "Walk", "Run", "Attack", "Hurt" };
std::string directions[4]{ "North", "South", "West", "East" };
Animator::Animator(Entity* entity, std::string text_name) {
	elapsed_time = 0;
	textures_name = text_name;
	this->entity = entity;
	last_state = "Idle";

	auto* cont = TextureContainer::getInstance();
	std::map<std::string, Animation*> dir_anim; // Animations by direction
	for (int i = 0; i < 4; i++) {
		dir_anim.emplace(directions[i], new Animation(text_name, states[0], directions[i], 1));
	}
	animations.emplace(states[0], dir_anim);

	dir_anim.clear();
	for (int i = 0; i < 4; i++) {
		dir_anim.emplace(directions[i], new Animation(text_name, states[1], directions[i], 1));
	}
	animations.emplace(states[1], dir_anim);

	dir_anim.clear();
	for (int i = 0; i < 4; i++) {
		dir_anim.emplace(directions[i], new Animation(text_name, states[2], directions[i], 0.5));
	}
	animations.emplace(states[2], dir_anim);

	dir_anim.clear();
	for (int i = 0; i < 4; i++) {
		dir_anim.emplace(directions[i], new Animation(text_name, states[3], directions[i], 0.5));
	}
	animations.emplace(states[3], dir_anim);

	dir_anim.clear();
	for (int i = 0; i < 4; i++) {
		dir_anim.emplace(directions[i], new Animation(text_name, states[4], directions[i], 0.5));
	}
	animations.emplace(states[4], dir_anim);
}

Animator::~Animator() {
	for (auto animation = animations.begin(); animation != animations.end(); animation++) {
		for (auto i = animation->second.begin(); i != animation->second.end(); i++) {
			delete i->second;
		}
	}
}

void Animator::setTexturesName(std::string text_name) {
	textures_name = text_name;
	
	for (auto animation = animations.begin(); animation != animations.end(); animation++) {
		for (auto i = animation->second.begin(); i != animation->second.end(); i++) {
			delete i->second;
		}
	}

	auto* cont = TextureContainer::getInstance();
	for (int i = 0; i < 5; i++) {
		std::map<std::string, Animation*> dir_anim; // Animations by direction
		for (int j = 0; j < 4; j++) {
			dir_anim[directions[j]] = new Animation(text_name, states[i], directions[j], 0.5);
		}
		animations[states[i]] =  dir_anim;
	}

}

std::string Animator::getTexturesName() {
	return textures_name;
}

void Animator::update(float delta_time) {
	elapsed_time += delta_time;
	std::string state = entity->getState()->getName();
	if (last_state != state) {
		last_state = state;
		elapsed_time = 0;
	}

	std::string direction = entity->getDirection();

	entity->setTexture(animations[state][direction]->getComponent(elapsed_time));

	if (elapsed_time > animations[state][direction]->getAnimationDuration()) {
		elapsed_time = 0;
		if (animations[state][direction]->isOneTime()) {
			entity->setState(new IdleState(entity));
		}
	}
}