#ifndef _MAINMENU_H_
#define _MAINMENU_H_
#include "Menu.h"
#include "PauseMenu.h"

class MainMenu : public Menu
{
public:
	MainMenu(sf::RenderWindow* window, Game* game);
	~MainMenu();

	virtual void update(sf::Event& event, Camera* camera, float delta_time) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private: 
	bool is_camera_switched;
	sf::RectangleShape* background[2];
	sf::Text title;

	std::vector<Button*> buttons;
};
#endif

