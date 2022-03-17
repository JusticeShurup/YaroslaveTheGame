#ifndef _SOUND_CONTAINER_H_
#define _SOUND_CONTAINER_H_
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <map>

class SoundContainer
{
public: 
	static SoundContainer* getInstance();

	sf::Sound* getSoundByName(std::string name);

	std::vector<sf::SoundBuffer*> getBuffers();
	std::vector<sf::Sound*> getSounds();
	std::vector<sf::Music*> getMenuMusic();

private:
	SoundContainer();

	std::vector<sf::SoundBuffer*> buffers;
	std::vector<sf::Sound*> sounds;
	std::vector<sf::Music*> menu_music;

	std::map<std::string, sf::Sound*> soundAlphabet;
	static SoundContainer* instance;
};
#endif

