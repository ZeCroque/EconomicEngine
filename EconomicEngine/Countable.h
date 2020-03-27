#ifndef COUNTABLE_H
#define COUNTABLE_H

#include "Tradable.h"

class Countable : public Tradable
{
private:
	int count;

public:
	Countable() : Tradable(), count(0){}
	Countable(const int count) : Tradable(), count(count){}
	int getCount() const;
};

#endif

