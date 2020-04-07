#ifndef WOOD_H
#define WOOD_H

#include "Countable.h"

class Wood : public Countable
{
public:
	Wood() : Countable()
	{
		name = "Wood";
	}
};

#endif
