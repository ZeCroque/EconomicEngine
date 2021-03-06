#include "Traders/CraftFactory.h"
#include "Tradables/Countable.h"
#include "Tradables/Uncountable/Uncountable.h"
#include "Traders/Trader.h"

CraftFactory::CraftFactory() : owner(nullptr)
{
}

CraftFactory::CraftFactory(CraftFactory& inCraftFactory) : CraftFactory()
{
	defaultObjects = std::map<size_t, Craft*>(inCraftFactory.defaultObjects);
}

void CraftFactory::registerCraft(Craft* inCraft)
{
	registerObject(inCraft->getResultId(), inCraft);
}

bool CraftFactory::isCraftable(const size_t itemId) const
{
	//Items required
	for (const auto& requirement : getDefaultObject(itemId)->getRequirements())
	{
		bool bRequirementsOwned = false;
		for(const auto& item : owner->getInventory())
		{
			if(item->getId() == requirement.first)
			{
				if (auto * countableItem = dynamic_cast<Countable*>(item.get()); countableItem && countableItem->getCount() >= requirement.second)
				{
					bRequirementsOwned = true;
				}
				break;
			}
		}
		if(!bRequirementsOwned)
		{
			return false;
		}
	}

	//Tools
	for (const auto& requiredTool : getDefaultObject(itemId)->getRequiredTools())
	{
		bool bHasBehavior = false;
		for (const auto& item : owner->getInventory())
		{
			if (auto * tool = dynamic_cast<Uncountable*>(item.get()); tool && tool->getBehavior() && tool->getBehavior()->getId() == requiredTool)
			{
				bHasBehavior = true;
				break;
			}
		}
		if (!bHasBehavior)
		{
			return false;
		}
	}
	
	return true;
}

void CraftFactory::setOwner(Trader* inOwner)
{
	owner = inOwner;
}

CraftFactory* CraftFactory::clone()
{
	return new CraftFactory(*this);
}

