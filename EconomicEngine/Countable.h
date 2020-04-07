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
	void decrementCountBy(int count);
	[[nodiscard]] int getCount() const;
};

//DEBUG
class Wheat : public Countable
{
public:
	Wheat();
	Wheat(const Wheat& wheat);
	Wheat* clone() override;
};

class Bread : public Countable
{
public:
	Bread();
	Bread(const Bread& bread);
	Bread* clone() override;
};

#endif

