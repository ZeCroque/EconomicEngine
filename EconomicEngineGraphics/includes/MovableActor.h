#ifndef MOVABLE_ACTOR
#define MOVABLE_ACTOR

#include <Signal.h>

#include "Actor.h"

class MovableActor : public Actor
{
public:
	Actor* clone() override;
	const Signal<>& getPathfindSucceededSignal() const;
protected:
	Signal<> pathfindSucceededSignal;
};


#endif //STATIC_ACTOR