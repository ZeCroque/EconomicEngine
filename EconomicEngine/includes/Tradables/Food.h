#ifndef FOOD_H
#define FOOD_H
#include "Countable.h"

class Food : public Countable
{
protected:
	float foodValue;
public:
	Food();
	Food(std::string name, std::pair<float, float> defaultPriceBelief, float foodValue, int count = 1);
	[[nodiscard]] float getFoodValue() const;

	Food* clone() override { return new Food(*this); }
};

#endif
