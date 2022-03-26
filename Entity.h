#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "GameObject.h"
#include "Map/Map.h"

class State;

class Animator;

class Game;

class Entity : public GameObject
{
protected: 
	Game* game;

public: 
	Entity();
	Entity(sf::Vector2f entity_size, sf::Vector2f hitbox_size);
	Entity(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name);
	virtual ~Entity();

	int sign(float value);

	void setGame(Game* game);
	Game* getGame() const;

	sf::Text* getNickname();

	sf::RectangleShape* getHealthBar();
	void updateHealthBar();
	sf::RectangleShape* getStaminaBar();
	void updateStaminaBar();

	void setName(std::string name);
	std::string getName() const;

	void setTexturesName(std::string textures_name);
	std::string getTexturesName();

	virtual void setTexture(std::string filename) override;
	virtual void setTexture(sf::Texture* texture) override;

	//Specifications
	void setHealthPointsValue(int value); // set HP value
	int getHealthPointsValue() const; // HP return value

	void setMaxHPValue(int value); // set HP limit value 
	int getMaxHPValue() const; // HP limit return value

	void setStaminaValue(float value); // set stamina value
	float getStaminaValue() const; // stamina return value

	void setMaxStaminaValue(int value); // set stamina limit value	
	int getMaxStaminaValue() const; // stamina limit return value
	float getStamPerTick() const;
	int getStamPerAttack() const;
	bool canLostStam() const;

	void setSpeedValue(float value); // set speed value 
	float getSpeedValue() const; // speed return value

	void setDamageValue(int value); // set damage value
	int getDamageValue(); // get damage value

	void setIsInFight(bool flag);
	bool isInFignt();
	//Specifications

	void setCanSwitchState(bool flag);
	bool canSwitchState();

	
	virtual void update(float delta_time);
	void updateAnimator(float delta_time, State* state);
	
	Animator* getAnimator();

	State* getState();
	void setState(State* state);

	std::string getDirection();
	void setDirection(std::string direction);

	float calcDistance(sf::Vector2f target_position); // Calculate distance to target from current position
	float calcDistance(sf::Vector2f target_position, sf::Vector2f own_position); // Calculate distance to target from smth position

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private: 
	sf::Text nickname;

	int health_points; 
	int max_health_points;
	float add_hp_timer;
	int add_hp_time; // time to add 1 HP
	
	float stamina_points;
	int max_stamina_points;
	float stam_per_tick;
	int stam_per_attack;
	bool can_lost_stam;

	bool is_in_fight;
	float in_fight_timer;

	float speed;
	int damage;

	float lost_stam_timer;
	float add_stam_timer;
	float switch_state_timer;
	bool can_switch_state;

	sf::RectangleShape* health_bar;
	sf::RectangleShape* stamina_bar;

	State* state;

	std::string direction;
	Animator* animator;
};
#endif