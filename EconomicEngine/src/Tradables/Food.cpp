#include "Tradables/Food.h"

Food::Food() : Countable(), foodValue(0.0f)
{
}

Food::Food(const std::string& inName, const std::pair<float, float> inDefaultPriceBelief, const float inFoodValue,
           const int inCount) : Countable(inName, inDefaultPriceBelief, inCount), foodValue(inFoodValue)
{
}

float Food::getFoodValue() const
{
	return foodValue;
}

Food* Food::clone()
{
	return new Food(*this);
}
