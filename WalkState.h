#ifndef _WALK_STATE_H_
#define _WALK_STATE_H_
#include "State.h"
class WalkState :
    public State
{
public: 
    WalkState(Entity* entity);
	~WalkState();

    virtual void update(float delta_time) override;
    virtual void update(float delta_time, Entity* target, Map* map) override;

private: 
	sf::SoundBuffer buffer;
	sf::Sound sound;
	float sound_timer;
	
    float add_stam_timer;

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