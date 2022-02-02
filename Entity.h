#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "GameObject.h"

class Animator;

class Entity : public GameObject
{
public: 
	Entity();
	Entity(sf::Vector2f entity_size, sf::Vector2f hitbox_size);
	Entity(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name);

	void setName(std::string name);
	std::string getName() const;


	sf::FloatRect getGlobalBounds();
	void setTexturesName(std::string textures_name);
	std::string getTexturesName();

	virtual void setTexture(std::string filename) override;
	virtual void setTexture(sf::Texture* texture) override;

	//Specifications
	void setHealthPointsValue(int value); // set HP value
	int getHealthPointsValue() const; // HP return value

	void setMaxHPValue(int value); // set HP limit value 
	int getMaxHPValue() const; // HP limit return value

	void setStaminaValue(int value); // set stamina value
	int getStaminaValue() const; // stamina return value

	void setMaxStaminaValue(int value); // set stamina limit value	
	int getMaxStaminaValue() const; // stamina limit return value

	void setSpeedValue(float value); // set speed value 
	float getSpeedValue(); // speed return value

	void setDamageValue(int value); // set damage value
	int getDamageValue(); // get damage value
	//Specifications

	virtual void update(float delta_time);
	void updateAnimator(float delta_time, std::string state);
	
	Animator* getAnimator();
	std::string getState();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private: 
	sf::Text nickname;

	int health_points; 
	int max_health_points;
	
	int stamina_points;
	int max_stamina_points;

	float speed;
	int damage;

	sf::RectangleShape* health_bar;
	sf::RectangleShape* stamina_bar;

	std::string state;
	Animator* animator;
};
#endif