#include <utility>

#include "Traders/Job.h"

Job::Job() : craftFactory(new CraftFactory()), id(0)
{
}

Job::Job(std::string inName) : Job()
{
	const std::hash<std::string> hash;
	
	name = std::move(inName);
	id = hash(name);
}

Job::Job(const Job& job) : Job()
{
	craftFactory = job.craftFactory->clone();
	usableTools = std::list<size_t>(job.usableTools);
	name = job.name;
	id = job.id;
}

Job::~Job()
{
	delete craftFactory;
}

void Job::addUsableTool(size_t toolId)
{
	usableTools.emplace_back(toolId);
}

Craft* Job::createCraft(const size_t inCraftId) const
{
	return craftFactory->createObject(inCraftId);
}

std::vector<size_t> Job::getCraftsIds() const
{
	std::vector<size_t> craftsIds;

	if (craftFactory->owner)
	{
		for (auto craftId : craftFactory->getKeys())
		{
			craftsIds.emplace_back(craftId);
		}
	}
	return craftsIds;
}

std::vector<size_t> Job::getCraftablesIds() const
{
	std::vector<size_t> craftablesIds;

	if(craftFactory->owner!=nullptr)
	{
		for (auto craftableId : craftFactory->getKeys())
		{
			if (craftFactory->isCraftable(craftableId))
			{
				craftablesIds.emplace_back(craftableId);
			}
		}
	}
	return craftablesIds;
}

std::vector<size_t> Job::getUncraftablesIds() const
{
	std::vector<size_t> uncraftablesIds;

	if (craftFactory->owner)
	{
		for (auto uncraftableId : craftFactory->getKeys())
		{
			if (!craftFactory->isCraftable(uncraftableId))
			{
				uncraftablesIds.emplace_back(uncraftableId);
			}
		}
	}
	return uncraftablesIds;
}

const std::list<size_t>& Job::getUsableTools() const
{
	return usableTools;
}

Craft* Job::getCraft(const size_t inCraftId) const
{
	return craftFactory->getDefaultObject(inCraftId);
}

size_t Job::getId() const
{
	return id;
}

std::string Job::getName() const
{
	return name;
}

CraftFactory* Job::getCraftFactory() const
{
	return craftFactory;
}

void Job::setOwner(Trader* inOwner) const
{
	craftFactory->setOwner(inOwner);
}

Job* Job::clone()
{
	return new Job(*this);
}
