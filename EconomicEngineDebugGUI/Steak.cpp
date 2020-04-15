#include "Steak.h"

#include <vcruntime_typeinfo.h>

Steak::Steak() : Food()
{
	this->id = typeid(Steak).hash_code();
	this->name = "Steak";
	this->foodValue = 6.0f;
	this->defaultPriceBelief = std::pair<float, float>(1.0f, 2.0f);
}

Steak* Steak::clone()
{
	return new Steak(*this);
}
