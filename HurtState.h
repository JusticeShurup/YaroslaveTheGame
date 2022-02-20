#ifndef _HURT_STATE_H_
#define _HURT_STATE_H_
#include "State.h"
class HurtState :
    public State
{
public: 
    HurtState(Entity* entity, float damage_taken);

    virtual void update(float delta_time) override;
    virtual void update(float delta_time, Entity* target, Map* map) override;
private: 
    float elapsed_dist; // Distance after getting damage to pass

    float switch_state_timer;
    float damage_taken;
    bool is_damage_received;
};
#endif