#include "StaticActor.h"

StaticActor::StaticActor(const std::string& inTextureName) : Actor(inTextureName) {}

Actor* StaticActor::clone()
{
	return new StaticActor(*this);
}
