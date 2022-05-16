#ifndef _PROFILE_MENU_H_
#define _PROFILE_MENU_H_

#include "Menu.h"
class ProfileMenu : Menu
{
public:
	ProfileMenu(sf::RenderWindow* window, Game* game);
	~ProfileMenu();


	void setActive(bool flag);
	virtual void update(sf::Event& event, Camera* camera, float delta_time) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	bool is_background_in_pos;
	bool is_in_starty;
	bool can_close;
	sf::RectangleShape background;
	sf::Text* pause_text;
	std::vector<Button*> buttons;
	std::vector<sf::Texture*> button_textures; // in 0 position - idle textures, in 1 position hover textures

	std::vector<bool> can_play_sounds;


};

#endif