#include "Actor.h"
#include "GameManager.h"

Actor::Actor(const std::string& inTextureName) : x(0), y(0)
{
    const std::hash<std::string> hash;
    textureId = hash(inTextureName);

    sprite.setTexture(GameManager::getInstance()->getTexture(textureId));
}

sf::Sprite& Actor::getSprite()
{
    return sprite;
}
