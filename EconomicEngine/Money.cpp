#include "Money.h"

Money::Money() : Countable() {}

Money::Money(float count) : Countable(count) {}

Money* Money::clone()
{
	return new Money(*this);
}


