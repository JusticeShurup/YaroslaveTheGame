#include "SoundContainer.h"
#include <fstream>
#include <iostream>

SoundContainer* SoundContainer::instance = nullptr;

SoundContainer::SoundContainer() {
	std::ifstream file("Sounds\\SoundAlphabet.txt");

	while (!file.eof()) {
		std::string cathegory;
		int size;

		file >> cathegory;
		file >> size;

		for (int i = 0; i < size; i++) {
			std::string filename;
			file >> filename;

			sf::SoundBuffer* buffer = new sf::SoundBuffer;
			buffer->loadFromFile(filename);
			sf::Sound* sound = new sf::Sound;
			sound->setBuffer(*buffer);

			buffers.push_back(buffer);
			sounds.push_back(sound);

			soundAlphabet[cathegory] = sound;
		}
	}
	file.close();

	file.open("Sounds\\MenuMusic.txt");

	while (!file.eof()) {
		std::string cathergory;
		int size;
		file >> cathergory >> size;
		for (int i = 0; i < size; i++) {
			std::string music_name;
			std::string music_filename;
			file >> music_name >> music_filename;
			sf::Music* music = new sf::Music;
			music->setVolume(50);
			if (!music->openFromFile(music_filename)) std::cout << "skip " << music_name << std::endl;
			menu_music.emplace_back(music);
		}

	}

	file.close();

}

sf::Sound* SoundContainer::getSoundByName(std::string name) {
	return soundAlphabet[name];
}

std::vector<sf::SoundBuffer*> SoundContainer::getBuffers() {
	return buffers;
}

std::vector<sf::Sound*> SoundContainer::getSounds() {
	return sounds;
}

std::vector<sf::Music*> SoundContainer::getMenuMusic() {
	return menu_music;
}

SoundContainer* SoundContainer::getInstance() {
	if (!SoundContainer::instance) {
		SoundContainer::instance = new SoundContainer;
	}

	return SoundContainer::instance;
}