#include "Leather.h"

#include <vcruntime_typeinfo.h>

Leather::Leather() : Countable()
{
	this->id = typeid(Leather).hash_code();
	this->name = "Leather";
	this->defaultPriceBelief = std::pair<float, float>(0.1f, 0.5f);
}

Leather* Leather::clone()
{
	return new Leather(*this);
}
