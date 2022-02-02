#include "Animation.h"
#include "TextureContainer/TextureContainer.h"
#include <iostream>

Animation::Animation(std::string text_name, std::string anim_name, std::string dir, float anim_dur) {
	textures_name = text_name;
	animation_name = anim_name;
	animation_duration = anim_dur;
	direction = dir;

	animation_components = TextureContainer::getInstance()->getEntityTextures(text_name, anim_name, direction);
}

float Animation::getAnimationDuration() const{
	return animation_duration;
}

sf::Texture* Animation::getComponent(int index) {
	if (index < 0 || index >= animation_components.size()) index = 0;
	return animation_components[index];
}

sf::Texture* Animation::getComponent(float elapsed_time) {
	float segmentation = animation_duration / animation_components.size();

	//BUG
	for (int i = 0; i < animation_components.size(); i++) {
		if (elapsed_time > segmentation * (i + 1) && elapsed_time < segmentation * (i + 2)) return animation_components[i];
	}
}