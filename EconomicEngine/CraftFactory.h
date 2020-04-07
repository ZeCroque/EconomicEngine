#ifndef CRAFT_FACTORY_H
#define CRAFT_FACTORY_H

#include "Craft.h"
#include "Trader.h"
#include "AbstractFactory.h"

class Trader;

class CraftFactory : public AbstractFactory<size_t, Craft>, public Clonable<CraftFactory>
{
	friend class Job;
	
private:
	Trader* owner;
public:
	CraftFactory();
	CraftFactory(CraftFactory& craftFactory);
	void setOwner(Trader* owner);
	[[nodiscard]] bool isCraftable(size_t key) const;
	void registerCraft(Craft* craft);
	CraftFactory* clone() override;
};


#endif
