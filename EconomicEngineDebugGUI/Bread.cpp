#include "Bread.h"

#include <vcruntime_typeinfo.h>

Bread::Bread() :Countable()
{
	this->id = typeid(Bread).hash_code();
	this->name = "Bread";
	this->defaultPriceBelief = std::pair<float, float>(0.5f, 1.0f);
}

Bread* Bread::clone()
{
	return new Bread(*this);
}
