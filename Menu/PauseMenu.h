#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_
#include "Menu.h"

class PauseMenu : public Menu
{
public:
	PauseMenu(sf::RenderWindow* window, Game* game);
	~PauseMenu();


	void setActive(bool flag);
	virtual void update(sf::Event& event, Camera* camera, float delta_time) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	bool is_background_in_pos;
	sf::RectangleShape background;

	std::vector<sf::RectangleShape*> buttons;
	std::vector<sf::Texture*> button_textures; // in 0 position - idle textures, in 1 position hover textures

	std::vector<bool> can_play_sounds;


};

#endif