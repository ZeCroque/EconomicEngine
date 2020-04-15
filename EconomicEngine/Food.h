#ifndef FOOD_H
#define FOOD_H
#include "Countable.h"

class Food : public Countable
{
protected:
	float foodValue;
public:
	Food();
	Food(float foodValue, int count);
	[[nodiscard]] float getFoodValue() const;
};

#endif
