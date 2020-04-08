#include "Steak.h"

#include <vcruntime_typeinfo.h>

Steak::Steak() : Food()
{
	this->id = typeid(Steak).hash_code();
	this->name = "Steak";
	this->foodValue = 3;
	this->defaultPriceBelief = std::pair<float, float>(0.5f, 1.0f);
}

Steak* Steak::clone()
{
	return new Steak(*this);
}
