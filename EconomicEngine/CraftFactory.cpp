#include "CraftFactory.h"

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

