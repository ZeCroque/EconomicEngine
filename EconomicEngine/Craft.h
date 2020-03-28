#ifndef CRAFT_H
#define CRAFT_H
#include <vector>

#include "Tradable.h"

//TODO reuse abstract factory

class Craft
{
private:
	float advancement;
	float baseRate;
	float currentRate;
	Tradable& craftResult;
	std::vector<Tradable> requirements;
public:
	bool canBeCrafted();
	Tradable advanceCraft();
};

#endif