#include "Wood.h"

#include <vcruntime_typeinfo.h>

Wood::Wood() : Countable()
{
	this->id = typeid(Wood).hash_code();
	this->name = "Wood";
	this->defaultPriceBelief = std::pair<float, float>(0.1f, 0.5f);
}

Wood* Wood::clone()
{
	return new Wood(*this);
}
