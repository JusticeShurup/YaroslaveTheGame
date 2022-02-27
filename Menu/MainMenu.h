#ifndef _MAINMENU_H_
#define _MAINMENU_H_
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu(sf::RenderWindow* window);
	~MainMenu();

	virtual void update(sf::Event& event, Camera* camera) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private: 
	bool is_camera_switched;
	sf::RectangleShape* background;
	sf::Text title;

	std::vector<Button*> buttons;
};
#endif

