#ifndef STEAK_H
#define STEAK_H

#include <string>

#include "Food.h"


class Steak : public Food
{
public:
	Steak() : Food()
	{
		foodValue = 2;
		name = "Steak";
	}
};

#endif
