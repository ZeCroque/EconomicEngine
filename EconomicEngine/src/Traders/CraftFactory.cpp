#include "Traders/CraftFactory.h"
#include "Tradables/Countable.h"
#include "Tradables/Uncountable/Uncountable.h"
#include "Traders/Trader.h"

CraftFactory::CraftFactory() : owner(nullptr){}

CraftFactory::CraftFactory(CraftFactory& craftFactory) : CraftFactory()
{
	this->defaultObjects = std::map<size_t, Craft*>(craftFactory.defaultObjects);
}

void CraftFactory::setOwner(Trader* NewOwner)
{
	owner = NewOwner;
}

bool CraftFactory::isCraftable(const size_t key) const
{
	//Items required
	for (const auto& requirement : getDefaultObject(key)->getRequirement())
	{
		bool requirementsOwned = false;
		for(const auto& item : owner->getInventory())
		{
			if(item->getId() == requirement.first)
			{
				auto* countableItem = dynamic_cast<Countable*>(item.get());
				if (countableItem != nullptr && countableItem->getCount() >= requirement.second)
				{
					requirementsOwned = true;
				}
				break;
			}
		}
		if(!requirementsOwned)
		{
			return false;
		}
	}

	//Tools
	for (const auto& requiredTool : getDefaultObject(key)->getToolsRequired())
	{
		bool hasBehavior = false;
		for (const auto& item : owner->getInventory())
		{
			auto* tool = dynamic_cast<Uncountable*>(item.get());
			if (tool != nullptr && tool->getBehavior() != nullptr && tool->getBehavior()->getId() == requiredTool)
			{
				hasBehavior = true;
				break;
			}
		}
		if (!hasBehavior)
		{
			return false;
		}
	}
	
	return true;
}

void CraftFactory::registerCraft(Craft* craft)
{
	registerObject(craft->getResult(), craft);
}

CraftFactory* CraftFactory::clone()
{
	return new CraftFactory(*this);
}

