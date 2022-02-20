#ifndef _IDLE_STATE_H_
#define _IDLE_STATE_H_

#include "State.h"

class IdleState :
    public State
{
public: 
    IdleState(Entity* entity);

    virtual void update(float delta_time) override;
    virtual void update(float delta_time, Entity* target, Map* map) override;

private: 
    float add_stam_timer;
};
#endif

