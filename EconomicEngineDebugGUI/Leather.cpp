#include "Leather.h"

#include <vcruntime_typeinfo.h>


Leather::Leather() : Countable()
{
	this->name = "Leather";
}

Leather::Leather(const Leather& leather) : Countable(leather)
{
	this->id = typeid(Leather).hash_code();
}

Leather* Leather::clone()
{
	return new Leather(*this);
}
