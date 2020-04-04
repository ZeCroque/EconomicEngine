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
	CraftFactory() = delete;
	CraftFactory(Trader& owner);
	[[nodiscard]] bool isCraftable(size_t key) const;
};


#endif
