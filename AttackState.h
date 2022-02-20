#ifndef _ATTACK_STATE_H_
#define _ATTACK_STATE_H_
#include "State.h"
class AttackState :
    public State
{
public: 
    AttackState(Entity* entity);

    virtual void update(float delta_time) override;
    virtual void update(float delta_time, Entity* target, Map* map) override;
private: 
    float switch_state_timer;
    bool is_stamina_lost;
};
#endif