#include "Meat.h"

#include <vcruntime_typeinfo.h>

Meat::Meat() : Countable()
{
	this->id = typeid(Meat).hash_code();
	this->name = "Meat";
	this->defaultPriceBelief = std::pair<float, float>(0.5f, 1.0f);
}

Meat* Meat::clone()
{
	return new Meat(*this);
}
