#include "Money.h"

Money::Money() : Countable() {}

Money::Money(int count) : Countable(count) {}

Money* Money::clone()
{
	return new Money(*this);
}


