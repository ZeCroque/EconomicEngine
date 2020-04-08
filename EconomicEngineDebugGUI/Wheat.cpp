#include "Wheat.h"

#include <vcruntime_typeinfo.h>


Wheat::Wheat() : Countable()
{
	this->name = "Wheat";
}

Wheat::Wheat(const Wheat& wheat) : Countable(wheat)
{
	this->id = typeid(Wheat).hash_code();
}

Wheat* Wheat::clone()
{
	return new Wheat(*this);
}