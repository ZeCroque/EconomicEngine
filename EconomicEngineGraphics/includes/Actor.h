#ifndef ACTOR_H
#define ACTOR_H

#include <SFML/Graphics/Sprite.hpp>
#include "Clonable.h"
#include <string>

class Actor : public Clonable<Actor>
{

public:
    explicit Actor(const std::string &textureName);

    sf::Sprite &getSprite();

    int x;
    int y;

private:
    sf::Sprite sprite;
    size_t textureId;
};

#endif //ACTOR_H
