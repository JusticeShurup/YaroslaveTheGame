#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class GameObject : public sf::Drawable
{
public:
	GameObject();
	GameObject(sf::Vector2f object_size, sf::Vector2f hitbox_size);
	GameObject(std::string filename, sf::Vector2f object_size, sf::Vector2f object_position, sf::Vector2f hitbox_size, sf::Vector2f hitbox_position, std::string name);
	GameObject(sf::Texture* texture, sf::Vector2f object_size, sf::Vector2f object_position, sf::Vector2f hitbox_size, sf::Vector2f hitbox_position, std::string name);
	GameObject(sf::Texture* texture, sf::Vector2f object_size, sf::Vector2f object_position, int hitbox_count, std::vector<sf::Vector2f> hitbox_sizes, std::vector<sf::Vector2f> hitbox_positions, std::string name);

	~GameObject();

	sf::RectangleShape* getObjectShape();
	sf::RectangleShape* getHitboxShape(); // Return the 1-st hitbox 
	sf::RectangleShape* getHitboxShape(int number); // Return the definite hitbox, if gameobject has only one hitbox, return him
	std::vector<sf::RectangleShape>* getHitboxShapes();

	int getHitboxCount();

	void setPosition(sf::Vector2f object_position);
	void setPosition(float object_x, float object_y);
	void setNewHitboxPosition(sf::Vector2f hitbox_position); // Set position of hitbox relative to object
	void setNewHitboxPositions(std::vector<sf::Vector2f> hitbox_positions); // Set positions of hitboxes relative to object

	sf::FloatRect getGlobalBounds() const;

	sf::Vector2f getObjectPosition() const;
	sf::Vector2f getHitboxPosition();
	sf::Vector2f getHitboxPosition(int number);
	std::vector<sf::Vector2f> getHitboxPositions();
	sf::Vector2f getCenter() const; // Works only for GameObjects with only 1 hitbox

	void showObjectOutlines();
	void showHitboxOutlines();
	
	virtual void setTexture(std::string filename);
	virtual void setTexture(sf::Texture* texture);
	sf::Texture* getTexture();

	void setName(std::string name);
	std::string getName();


	void hideHitboxes(bool flag);
	bool getHitboxesHidden();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:	
	std::string name;

	sf::Texture* texture;

	sf::RectangleShape* object;

	std::vector<sf::RectangleShape> hitboxes;
	std::vector<sf::Vector2f> hitbox_delta_positions;

	bool is_load_from_file;
	bool is_hitboxes_hidden;
};
#endif