#include "SoundContainer.h"
#include <fstream>

SoundContainer* SoundContainer::instance = nullptr;

SoundContainer::SoundContainer() {
	std::ifstream file("Sounds\\SoundAlphabet.txt");

	while (!file.eof()) {
		std::string cathegory;
		int size;

		file >> cathegory;
		if (cathegory == "\0") {
			break;
		}
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

SoundContainer* SoundContainer::getInstance() {
	if (!SoundContainer::instance) {
		SoundContainer::instance = new SoundContainer;
	}

	return SoundContainer::instance;
}