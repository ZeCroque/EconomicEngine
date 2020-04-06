#ifndef COUNTABLE_H
#define COUNTABLE_H

#include "Tradable.h"

class Countable : public Tradable
{
protected:
	int count;

public:
	Countable();
	Countable(const Countable& countable);
	Countable(int count);
	void incrementCountBy(int count);
	[[nodiscard]] int getCount() const;
};

//DEBUG
class Wheat : public Countable
{
	Wheat* clone() override;
};

class Bread : public Countable
{
	Bread* clone() override;
};

#endif

