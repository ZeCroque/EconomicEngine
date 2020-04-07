#include "Food.h"

Food::Food() : Countable(), foodValue(0){}

Food::Food(const int foodValue, const int count = 1) : Countable(count), foodValue(foodValue){}
