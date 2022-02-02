#include "Animator.h"
#include "TextureContainer/TextureContainer.h"
#include "Entity.h"
#include <iostream>

std::string states[5]{ "Idle", "Walk", "Run", "Attack", "Hurt" };
std::string directions[4]{ "North", "South", "West", "East" };
Animator::Animator(Entity* entity, std::string text_name) {
	elapsed_time = 0;
	textures_name = text_name;
	this->entity = entity;

	auto* cont = TextureContainer::getInstance();
	for (int i = 0; i < 5; i++) {
		std::map<std::string, Animation*> dir_anim; // Animations by direction
		for (int j = 0; j < 4; j++) {
			dir_anim.emplace(directions[j], new Animation(text_name, states[i], directions[j], 2));
		}
		animations.emplace(states[i], dir_anim);
	}
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
			dir_anim.emplace(directions[j], new Animation(text_name, states[i], directions[j], 0.1));
		}
		animations.emplace(states[i], dir_anim);
	}

}

std::string Animator::getTexturesName() {
	return textures_name;
}

void Animator::update(float delta_time) {
	elapsed_time += delta_time;
	std::string state = entity->getState();
	/*
	int segmentation = animations[state]["South"]->getAnimationDuration() / 4;
	for (int i = 0; i < 4; i++) {
		std::cout << i << " " << elapsed_time << " " << segmentation * (i + 1) << " " << animations[state]["South"]->getAnimationDuration() / 4 << std::endl;
	}
	*/

	entity->setTexture(animations[state]["South"]->getComponent(elapsed_time));

	if (elapsed_time > animations[state]["South"]->getAnimationDuration()) {
		elapsed_time = 0;
	}
}