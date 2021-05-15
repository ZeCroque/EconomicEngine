#ifndef TRADABLE_H
#define TRADABLE_H

#include <string>

#include "Clonable.h"

class Trader;

class Tradable : public Clonable<Tradable>
{
protected:
	size_t id;
	std::string name;
	std::pair<float, float> defaultPriceBelief;
	Trader* owningTrader;

public:
	Tradable();
	Tradable(std::string inName, std::pair<float, float> inDefaultPriceBelief);
	Tradable(const Tradable& inTradable);

	virtual void setOwningTrader(class Trader* inOwningTrader);
	void removeFromOwnerInventory();

	[[nodiscard]] std::string getName() const;
	[[nodiscard]] Trader* getOwningTrader() const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] const std::pair<float, float>& getDefaultPriceBelief() const;
};
#endif
