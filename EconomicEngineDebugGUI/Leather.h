#ifndef LEATHER_H
#define LEATHER_H

#include "Countable.h"

class Leather : public Countable
{
public:
	Leather() : Countable()
	{
		name = "Leather";
	}
};

#endif
