#ifndef IRONORE_H
#define IRONORE_H

#include "Countable.h"

class IronOre : public Countable
{
public:
	IronOre() : Countable()
	{
		name = "Iron Ore";
	}
};

#endif
