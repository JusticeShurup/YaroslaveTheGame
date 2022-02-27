#ifndef _MENU_H_
#define _MENU_H_
#include "../Button.h"
#include "../Camera.h"

class Menu : public sf::Drawable
{
public: 
	Menu(sf::RenderWindow* window);

	void setActive(bool flag);
	bool isActive();

	sf::RenderWindow* getWindow();

	virtual void update(sf::Event& event, Camera* camera) = 0;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private: 
	bool is_active;
	
	sf::RenderWindow* window;

};
#endif
