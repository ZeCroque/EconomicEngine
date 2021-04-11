#include "Food.h"

Food::Food() : Countable(), foodValue(0.0f){}

Food::Food(std::string name, std::pair<float, float> defaultPriceBelief, const float foodValue, const int count) : Countable(name, defaultPriceBelief, count), foodValue(foodValue){}

float Food::getFoodValue() const
{
	return foodValue;
}
