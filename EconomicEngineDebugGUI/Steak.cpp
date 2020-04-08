#include "Steak.h"

#include <vcruntime_typeinfo.h>


Steak::Steak() : Food()
{
	this->name = "Steak";
	this->foodValue = 5;
}

Steak::Steak(const Steak& steak) : Food(steak)
{
	this->id = typeid(Steak).hash_code();
}

Steak* Steak::clone()
{
	return new Steak(*this);
}
