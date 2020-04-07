#ifndef BREAD_H
#define BREAD_H

#include <string>

#include "Food.h"


class Bread : public Food
{
public:
	Bread() : Food()
	{
		foodValue = 2;
		name = "Bread";
	}
};

#endif
