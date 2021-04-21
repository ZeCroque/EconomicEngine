//
// Created by relin on 21/04/2021.
//

#include "Actor.h"
#include "GameManager.h"

Actor::Actor(const std::string &textureName) : x(0), y(0)
{
    const std::hash<std::string> hash;
    textureId = hash(textureName);

    sprite.setTexture(GameManager::getInstance()->getTexture(textureId));
}

sf::Sprite &Actor::getSprite()
{
    return sprite;
}
