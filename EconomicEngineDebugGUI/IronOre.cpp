#include "IronOre.h"

#include <vcruntime_typeinfo.h>


IronOre::IronOre() : Countable()
{
	this->name = "Iron Ore";
}

IronOre::IronOre(const IronOre& ironOre) : Countable(ironOre)
{
	this->id = typeid(IronOre).hash_code();
}

IronOre* IronOre::clone()
{
	return new IronOre(*this);
}
