#ifndef STATIC_ACTOR
#define STATIC_ACTOR
#include "Actor.h"

class StaticActor : public Actor
{
public:
	Actor* clone() override;
};

#endif //STATIC_ACTOR