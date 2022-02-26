#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Entity.h"
class Player : public Entity
{
public: 
	Player();
	Player(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name);

	void update(sf::Event& event, float delta_time, Map* map);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:

	int level;
	int current_xp;

	bool damage_delivered;
};
#endif
