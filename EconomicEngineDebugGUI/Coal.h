#ifndef COAL_H
#define COAL_H

#include "Countable.h"

class Coal : public Countable
{
public:
	Coal() : Countable()
	{
		name = "Coal";
	}
};

#endif
