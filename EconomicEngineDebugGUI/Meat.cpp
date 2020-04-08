#include "Meat.h"

#include <vcruntime_typeinfo.h>


Meat::Meat() : Countable()
{
	this->name = "Meat";
}

Meat::Meat(const Meat& meat) : Countable(meat)
{
	this->id = typeid(Meat).hash_code();
}

Meat* Meat::clone()
{
	return new Meat(*this);
}
