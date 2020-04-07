#ifndef MEAT_H
#define MEAT_H

#include "Countable.h"

class Meat : public Countable
{
public:
	Meat() : Countable()
	{
		name = "Meat";
	}
};

#endif
