#ifndef ACTOR_H
#define ACTOR_H

#include <SFML/Graphics/Sprite.hpp>
#include <string>

#include "Clonable.h"

class Actor : public Clonable<Actor>
{

public:
    explicit Actor(const std::string& inTextureName);

    sf::Sprite& getSprite();

    int x;
    int y;

private:
    sf::Sprite sprite;
    size_t textureId;
};

#endif //ACTOR_H
