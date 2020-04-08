#include "Bread.h"

#include <vcruntime_typeinfo.h>


Bread::Bread() : Food()
{
	this->name = "Bread";
	this->foodValue = 3;
}

Bread::Bread(const Bread& bread) : Food(bread)
{
	this->id = typeid(Bread).hash_code();
}

Bread* Bread::clone()
{
	return new Bread(*this);
}
