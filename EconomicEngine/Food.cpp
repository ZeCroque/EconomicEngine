#include "Food.h"

Food::Food() : Countable(), foodValue(0){}

Food::Food(const int foodValue, const float count = 1.0f) : Countable(count), foodValue(foodValue){}
