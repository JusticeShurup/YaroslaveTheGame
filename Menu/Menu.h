#ifndef _MENU_H_
#define _MENU_H_
#include "../Button.h"
#include "../Camera.h"
class Game;

class Menu : public sf::Drawable
{
public: 
	Menu(sf::RenderWindow* window, Game* game);

	virtual void setActive(bool flag);
	bool isActive();

	Game* getGame() const;

	sf::RenderWindow* getWindow();

	virtual void update(sf::Event& event, Camera* camera, float delta_time) = 0;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private: 
	bool is_active;
	
	sf::RenderWindow* window;
	Game* game;
};
#endif
