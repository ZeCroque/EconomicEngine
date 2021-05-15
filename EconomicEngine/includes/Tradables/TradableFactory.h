#ifndef TRADABLE_FACTORY_H
#define TRADABLE_FACTORY_H

#include <string>

#include "AbstractFactory.h"
#include "Tradable.h"

class TradableFactory : public AbstractFactory<size_t, Tradable>
{
public:
	[[nodiscard]] std::vector<std::string> getTradablesName() const
	{
		auto keys = getKeys();
		std::vector<std::string> names;
		names.reserve(keys.size());
	
		for(auto key : keys)
		{
			names.emplace_back(getDefaultObject(key)->getName());
		}
		return names;
	}
};

#endif //TRADABLE_FACTORY_H