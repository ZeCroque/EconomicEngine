#include "Wheat.h"

#include <vcruntime_typeinfo.h>

Wheat::Wheat() : Countable()
{
	this->id = typeid(Wheat).hash_code();
	this->name = "Wheat";
	this->defaultPriceBelief = std::pair<float, float>(0.25f, 0.5f);
}

Wheat* Wheat::clone()
{
	return new Wheat(*this);
}
