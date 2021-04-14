#ifndef MOVABLE_ACTOR
#define MOVABLE_ACTOR

#include "Actor.h"

class MovableActor : public Actor
{
public:
	Actor* clone() override;
};


#endif //STATIC_ACTOR