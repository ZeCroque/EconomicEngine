#include "Bread.h"

#include <vcruntime_typeinfo.h>

Bread::Bread() : Food()
{
	this->id = typeid(Bread).hash_code();
	this->name = "Bread";
	this->foodValue = 3.0f;
	this->defaultPriceBelief = std::pair<float, float>(1.0f, 2.0f);
}

Bread* Bread::clone()
{
	return new Bread(*this);
}
