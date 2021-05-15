#ifndef TRADABLE_FACTORY_H
#define TRADABLE_FACTORY_H

#include <string>

#include "AbstractFactory.h"
#include "Tradable.h"

class TradableFactory : public AbstractFactory<size_t, Tradable>
{
public:
	[[nodiscard]] std::vector<std::string> getTradablesNames() const
	{
		auto tradablesIds = getKeys();
		std::vector<std::string> tradableNames;
		tradableNames.reserve(tradablesIds.size());
	
		for(auto tradableId : tradablesIds)
		{
			tradableNames.emplace_back(getDefaultObject(tradableId)->getName());
		}
		return tradableNames;
	}
};

#endif //TRADABLE_FACTORY_H