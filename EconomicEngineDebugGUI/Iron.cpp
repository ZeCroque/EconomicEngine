#include "Iron.h"

#include <vcruntime_typeinfo.h>

Iron::Iron() : Countable()
{
	this->id = typeid(Iron).hash_code();
	this->name = "Iron";
	this->defaultPriceBelief = std::pair<float, float>(0.1f, 0.5f);
}

Iron* Iron::clone()
{
	return new Iron(*this);
}
