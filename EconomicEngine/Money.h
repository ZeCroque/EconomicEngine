#ifndef MONEY_H
#define MONEY_H
#include <type_traits>

#include "Countable.h"

class Money : public Countable
{
public:
	Money() : Countable(){}
	Money(int count) : Countable(count){}
};

#endif