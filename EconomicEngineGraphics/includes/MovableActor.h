#ifndef MOVABLE_ACTOR
#define MOVABLE_ACTOR

#include "Actor.h"

class MovableActor : public Actor
{
public:
    explicit MovableActor(const std::string &textureName) : Actor(textureName)
    {}

    Actor *clone() override;
};


#endif //STATIC_ACTOR