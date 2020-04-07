#include "TradableManager.h"
#include <typeinfo>

void TradableManager::registerTradable(Tradable* tradable)
{
	tradableFactory.registerObject(typeid(*tradable).hash_code(), tradable);
}

Tradable* TradableManager::createTradable(const size_t key) const
{
	return tradableFactory.createObject(key);
}

std::vector<std::string> TradableManager::getTradablesName() const
{
	auto keys = tradableFactory.getKeys();
	std::vector<std::string> names(keys.size());
	for(auto key : keys)
	{
		names.emplace_back(tradableFactory.getDefaultObject(key)->getName());
	}
	return names;
}

std::vector<size_t> TradableManager::getKeys() const
{
	return tradableFactory.getKeys();
}