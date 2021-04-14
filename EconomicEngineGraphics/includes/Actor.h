#ifndef ACTOR_H
#define ACTOR_H

#include <SFML/Graphics/Sprite.hpp>

#include "Clonable.h"

class Actor : public Clonable<Actor>
{
    sf::Sprite sprite;
public:
    int x;
    int y;
};

#endif //ACTOR_H
