#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "Entity.h"
class Enemy : public Entity
{
public: 
	Enemy(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name);
	~Enemy();

	void update(float delta_time, Entity* target, Map* map);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private: 
	bool can_move;
	bool can_attack;
	float time_after_attack;

	bool walk_only_X;
	float elapsed_dist_x;
	float dist_x;
	int sign_x;

	bool walk_only_Y;
	float elapsed_dist_y;
	float dist_y;
	int sign_y;
};
#endif