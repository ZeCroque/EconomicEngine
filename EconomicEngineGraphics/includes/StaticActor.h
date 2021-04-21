#ifndef STATIC_ACTOR
#define STATIC_ACTOR

#include "Actor.h"

class StaticActor : public Actor
{
public:
    explicit StaticActor(const std::string &textureName) : Actor(textureName)
    {}

    Actor *clone() override;
};

#endif //STATIC_ACTOR
