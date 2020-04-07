#ifndef TRADABLE_MANAGER_H
#define TRADABLE_MANAGER_H
#include <string>

#include "Singleton.h"
#include "TradableFactory.h"

class TradableManager : public Singleton<TradableManager>
{
private:
	TradableFactory tradableFactory;
public:
	void registerTradable(Tradable* tradable);
	[[nodiscard]] Tradable* createTradable(size_t key) const;
	[[nodiscard]] std::vector<std::string> getTradablesName() const;
};

#endif