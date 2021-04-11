#include "Tradables/TradableManager.h"

void TradableManager::registerTradable(Tradable* tradable)
{
	tradableFactory.registerObject(tradable->getId(), tradable);
}

Tradable* TradableManager::createTradable(const size_t key) const
{
	return tradableFactory.createObject(key);
}

std::vector<std::string> TradableManager::getTradablesName() const
{
	auto keys = tradableFactory.getKeys();
	std::vector<std::string> names;
	names.reserve(keys.size());

	
	for(auto key : keys)
	{
		names.emplace_back(tradableFactory.getDefaultObject(key)->getName());
	}
	return names;
}

Tradable* TradableManager::getTradable(const size_t key) const
{
	return tradableFactory.getDefaultObject(key);
}

std::vector<size_t> TradableManager::getKeys() const
{
	return tradableFactory.getKeys();
}
