#include "CraftFactory.h"

#include "Countable.h"
#include "Trader.h"

CraftFactory::CraftFactory() : owner(nullptr){}

CraftFactory::CraftFactory(CraftFactory& craftFactory) : CraftFactory()
{
	this->defaultObjects = std::map<size_t, Craft*>(craftFactory.defaultObjects);
}

void CraftFactory::setOwner(Trader* owner)
{
	this->owner = owner;
}

bool CraftFactory::isCraftable(const size_t key) const
{
	if(owner!=nullptr)
	{
		bool requirementsOwned = true;

		//Items required
		for (const auto requirement : this->getDefaultObject(key)->getRequirement())
		{
			auto it = owner->getInventory().begin();

			for (; it != owner->getInventory().end(); ++it)
			{
				if (typeid(*it->get()).hash_code() == requirement.first)
				{
					auto* countableItem = dynamic_cast<Countable*>(it->get());
					if (countableItem != nullptr && countableItem->getCount() >= requirement.second)
					{
						break;
					}

				}
			}
			if (it == owner->getInventory().end())
			{
				requirementsOwned = false;
				break;
			}
		}

		//ToolsRequired
		auto toolsRequired = this->getDefaultObject(key)->getToolsRequired();
		if(!toolsRequired.empty())
		{
			auto it2 = toolsRequired.begin();
			for (; it2 != toolsRequired.end(); ++it2)
			{
				bool hasBehavior = false;
				for(const auto& item : owner->getInventory())
				{
					auto* tool = dynamic_cast<Uncountable*>(item.get());
					if(tool != nullptr && tool->getBehavior()!=nullptr && tool->getBehavior()->getId() == *it2)
					{
						hasBehavior = true;
						break;
					}
				}
				if(hasBehavior)
				{
					break;
				}
			}
			if (it2 == toolsRequired.end())
			{
				requirementsOwned = false;
			}
		}
		
		return requirementsOwned;
	}
	return false;
}

void CraftFactory::registerCraft(Craft* craft)
{
	registerObject(craft->getResult(), craft);
}

CraftFactory* CraftFactory::clone()
{
	return new CraftFactory(*this);
}

