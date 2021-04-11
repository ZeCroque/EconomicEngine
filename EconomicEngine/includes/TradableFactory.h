#ifndef TRADABLE_FACTORY_H
#define TRADABLE_FACTORY_H
#include "AbstractFactory.h"
#include "Tradable.h"

class TradableFactory : public AbstractFactory<size_t, Tradable>
{
	friend class TradableManager;
};

#endif
