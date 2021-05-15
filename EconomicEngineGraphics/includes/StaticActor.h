#ifndef STATIC_ACTOR_H
#define STATIC_ACTOR_H

#include "Actor.h"

class StaticActor : public Actor
{
public:
    explicit StaticActor(const std::string& inTextureName);

    Actor* clone() override;
};

#endif //STATIC_ACTOR_H
