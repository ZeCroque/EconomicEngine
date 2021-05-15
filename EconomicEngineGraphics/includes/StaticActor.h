#ifndef STATIC_ACTOR
#define STATIC_ACTOR

#include "Actor.h"

class StaticActor : public Actor
{
public:
    explicit StaticActor(const std::string& inTextureName);

    Actor *clone() override;
};

#endif //STATIC_ACTOR
