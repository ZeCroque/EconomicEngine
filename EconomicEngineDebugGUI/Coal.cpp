#include "Coal.h"

#include <vcruntime_typeinfo.h>

Coal::Coal() : Countable()
{
	this->id = typeid(Coal).hash_code();
	this->name = "Coal";
	this->defaultPriceBelief = std::pair<float, float>(0.1f, 0.5f);
}

Coal* Coal::clone()
{
	return new Coal(*this);
}
