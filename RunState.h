#ifndef _RUN_STATE_H_
#define _RUN_STATE_H_

#include "State.h"

class RunState 
	: public State
{
public: 
	RunState(Entity* entity);

	virtual void update(float delta_time) override;
	virtual void update(float delta_time, Entity* target, Map* map);

private:
	float lost_stam_timer;
	
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

