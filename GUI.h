#ifndef _GUI_H_
#define _GUI_H_
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Entity;
class Player;

class Camera;

class GUI : public sf::Drawable
{
public:

	GUI(Player* player);
	~GUI();

	bool isActive();

	void update(sf::Event& event, Camera* camera, float delta_time);
	void draw(sf::RenderTarget& window, sf::RenderStates states) const;
private: 

	float open_timer;
	bool active;

	Player* player;

	sf::RectangleShape background;
	sf::RectangleShape player_icon;

	sf::Text* atributes_text[4]; // 0 - Strength, 1 - Fortitude, 2 - Stamina, 3 - Free points
	sf::RectangleShape atribute_text_shapes[4];
	sf::Text* atributes_level_text[4];
	sf::RectangleShape atribute_text_level_shapes[4];
	int atributes_level[4];
	sf::Texture* button_texture;
	sf::RectangleShape buttons[3];

	sf::Text* xp_text;
	sf::Text* current_xp;
	sf::Text* xp_to_next;
	sf::RectangleShape xp_shape_empty;
	sf::RectangleShape xp_shape_full;

	sf::RectangleShape health_bar_empty;
	sf::RectangleShape health_bar_full;
	sf::Text* current_hp;
	sf::Texture* texture_hp;
	sf::RectangleShape health;

	sf::RectangleShape stamina_bar_empty;
	sf::RectangleShape stamina_bar_full;
	sf::Text* current_stamina;
	sf::Texture* stamina_texture;
	sf::RectangleShape stamina;

	sf::RectangleShape inventory_shapes[6];

	Entity* player_for_anim;
	
};
#endif