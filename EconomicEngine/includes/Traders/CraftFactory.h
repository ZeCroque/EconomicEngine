#ifndef CRAFT_FACTORY_H
#define CRAFT_FACTORY_H

#include "Craft.h"
#include "AbstractFactory.h"

class Trader;

class CraftFactory : public AbstractFactory<size_t, Craft>, public Clonable<CraftFactory>
{
	friend class Job;
	

public:
	CraftFactory();
	
	CraftFactory(CraftFactory& inCraftFactory);

	void registerCraft(Craft* inCraft);
	
	[[nodiscard]] bool isCraftable(size_t itemId) const;

	void setOwner(Trader* inOwner);
	
	CraftFactory* clone() override;

private:
	Trader* owner;
};


#endif //CRAFT_FACTORY_H
