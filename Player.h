#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Entity.h"
#include "GUI.h"

class Player : public Entity
{
public: 
	Player();
	Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name, GUI* gui);

	void setGUI(GUI* gui);

	void lockNearestTarget(Map* map);

	//void keyboardUpdate(sf::Event& event, float delta_time, Map* map); Ошибки молодости
	void update(sf::Event& event, float delta_time, Map* map);

	int getXP() const;
	int getXPToNextLevel() const;
	void setXP(int xp);
	void addXP(int xp);
	void lostXP(int xp);

	void addAtributeByName(std::string name, int points = 1);
	int getAtributeByName(std::string name);
	void addStrength(int points = 1);
	int getStrength() const;
	void addFortitude(int points = 1);
	int getFortitude() const;
	void addStamina(int points = 1);
	int getStamina() const;
	void addFreeAtributePoints(int points = 1);
	void setFreeAtributePoints(int points);
	int getFreeAtributePoints() const;


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void setDirection(std::string dir = "South");

	float dx; 
	float dy;

	sf::Text* player_name;
	sf::RectangleShape* target;
	Entity* entity_target;
	float target_lock_timer;

	GUI* gui;

	int level;
	int current_xp;
	int xp_to_nextlevel;

	// XP showing 
	std::vector<sf::Text*> xp_texts;
	std::vector<float> elapsed_time;
	std::vector<bool> text_animation_continue;

	sf::Text* initXPText(std::string string, sf::Vector2f position);
	void updateText(float anim_time, float elapsed_time, float delta_time, sf::Text*, std::vector<bool>& vector, int number);

	//Atributes

	int strength;
	int fortitude;
	int stamina; // Player level of Stamina

	int free_atribute_points;
	//Atributes


	bool damage_delivered;
};
#endif