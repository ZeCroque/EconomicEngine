#include "Iron.h"

#include <vcruntime_typeinfo.h>

Iron::Iron() : Countable()
{
	this->id = typeid(Iron).hash_code();
	this->name = "Iron";
	this->defaultPriceBelief = std::pair<float, float>(2.0f, 3.0f);
}

Iron* Iron::clone()
{
	return new Iron(*this);
}
