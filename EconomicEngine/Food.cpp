#include "Food.h"

Food::Food() : Countable(), foodValue(0.0f){}

Food::Food(const float foodValue, const int count = 1) : Countable(count), foodValue(foodValue){}

float Food::getFoodValue() const
{
	return foodValue;
}
