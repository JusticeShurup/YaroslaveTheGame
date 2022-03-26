#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Entity.h"
#include "GUI.h"

class Player : public Entity
{
public: 
	Player();
	Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name);

	void lockNearestTarget(Map* map);
	void update(sf::Event& event, float delta_time, Map* map);

	int getXP() const;
	int getXPToNextLevel() const;
	void setXP(int xp);
	void addXP(int xp);
	void lostXP(int xp);

	void addStrength(int points = 1);
	int getStrength() const;
	void addFortitude(int points = 1);
	int getFortitude() const;
	void addStamina(int points = 1);
	int getStamina() const;
	void addFreeAtributePoints(int points = 1);
	int getFreeAtributePoints() const;


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void setDirection(std::string dir);

	sf::Text* player_name;
	sf::Vector2f player_name_pos;
	GUI* inventory;
	sf::RectangleShape* target;
	Entity* entity_target;
	float target_lock_timer;

	int level;
	int current_xp;
	int xp_to_nextlevel;

	//Atributes

	int strength;
	int fortitude;
	int stamina; // Player level of Stamina

	int free_atribute_points;
	//Atributes


	bool damage_delivered;
};
#endif
