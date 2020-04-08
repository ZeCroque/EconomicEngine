#include "Coal.h"

#include <vcruntime_typeinfo.h>


Coal::Coal() : Countable()
{
	this->name = "Coal";
}

Coal::Coal(const Coal& coal) : Countable(coal)
{
	this->id = typeid(Coal).hash_code();
}

Coal* Coal::clone()
{
	return new Coal(*this);
}
