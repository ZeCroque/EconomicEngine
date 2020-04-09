#ifndef FOOD_H
#define FOOD_H
#include "Countable.h"

class Food : public Countable
{
protected:
	int foodValue;
public:
	Food();
	Food(int foodValue, int count);
};

#endif
