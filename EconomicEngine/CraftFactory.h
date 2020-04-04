#ifndef CRAFT_FACTORY_H
#define CRAFT_FACTORY_H

#include "Craft.h"
#include "Trader.h"
#include "AbstractFactory.h"

class Trader;

class CraftFactory : public AbstractFactory<size_t, Craft>
{
private:
	Trader& owner;
public:
	CraftFactory() = default;
	CraftFactory(Trader& owner);
	bool isCraftable(size_t key);
	Craft* craft(size_t key);
};


#endif
