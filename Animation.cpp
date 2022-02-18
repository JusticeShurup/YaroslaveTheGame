#include "Animation.h"
#include "TextureContainer/TextureContainer.h"
#include <iostream>

Animation::Animation(std::string text_name, std::string anim_name, std::string dir, float anim_dur) {
	textures_name = text_name;
	animation_name = anim_name;
	animation_duration = anim_dur;
	direction = dir;
	is_onetime = false;

	animation_components = TextureContainer::getInstance()->getEntityTextures(text_name, anim_name, direction);
}

Animation::Animation(std::string text_name, std::string anim_name, std::string dir, float anim_dur, bool is_onetime) {
	textures_name = text_name;
	animation_name = anim_name;
	animation_duration = anim_dur;
	direction = dir;
	is_onetime = true;

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

	if (elapsed_time >= animation_duration) return animation_components[0];
	return animation_components[int(elapsed_time / segmentation)];
}

bool Animation::isOneTime() const{
	return is_onetime;
}