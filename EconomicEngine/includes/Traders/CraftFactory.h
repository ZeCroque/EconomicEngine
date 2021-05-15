#ifndef CRAFT_FACTORY_H
#define CRAFT_FACTORY_H

#include "Craft.h"
#include "AbstractFactory.h"

class Trader;

class CraftFactory : public AbstractFactory<size_t, Craft>, public Clonable<CraftFactory>
{
	friend class Job;
	
private:
	Trader* owner;
public:
	CraftFactory();
	CraftFactory(CraftFactory& inCraftFactory);
	void setOwner(Trader* inOwner);
	[[nodiscard]] bool isCraftable(size_t inKey) const;
	void registerCraft(Craft* inCraft);
	CraftFactory* clone() override;
};


#endif
