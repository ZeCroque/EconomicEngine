#ifndef FOOD_H
#define FOOD_H

#include "Countable.h"

class Food : public Countable
{
protected:
	float foodValue;
public:
	Food();
	Food(const std::string& inName, std::pair<float, float> inDefaultPriceBelief, float inFoodValue, int inCount = 1);
	[[nodiscard]] float getFoodValue() const;

	Food* clone() override { return new Food(*this); }
};

#endif //FOOD_H
