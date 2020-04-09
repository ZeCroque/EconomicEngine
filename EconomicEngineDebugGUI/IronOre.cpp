#include "IronOre.h"

#include <vcruntime_typeinfo.h>

IronOre::IronOre() : Countable()
{
	this->id = typeid(IronOre).hash_code();
	this->name = "Iron Ore";
	this->defaultPriceBelief = std::pair<float, float>(0.1f, 0.5f);
}

IronOre* IronOre::clone()
{
	return new IronOre(*this);
}
