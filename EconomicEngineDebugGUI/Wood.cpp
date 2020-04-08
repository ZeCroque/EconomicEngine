#include "Wood.h"

#include <vcruntime_typeinfo.h>


Wood::Wood() : Countable()
{
	this->name = "Wood";
}

Wood::Wood(const Wood& wood) : Countable(wood)
{
	this->id = typeid(Wood).hash_code();
}

Wood* Wood::clone()
{
	return new Wood(*this);
}
