#include "Traders/Job.h"

#include <utility>

Job::Job() : craftFactory(new CraftFactory()), id(0){}

Job::Job(std::string newName) : Job()
{
	const std::hash<std::string> hash;
	
	name = std::move(newName);
	id = hash(name);
}

Job::Job(const Job& job) : Job()
{
	craftFactory = job.craftFactory->clone();
	usableToolsList = std::list<size_t>(job.usableToolsList);
	name = job.name;
	id = job.id;
}

Job::~Job()
{
	delete craftFactory;
}

void Job::setOwner(Trader* owner) const
{
	craftFactory->setOwner(owner);
}

Craft* Job::createCraft(const size_t typeId) const
{
	return craftFactory->createObject(typeId);
}

std::vector<size_t> Job::getCraftList() const
{
	std::vector<size_t> craftableList;

	if (craftFactory->owner != nullptr)
	{
		for (auto key : craftFactory->getKeys())
		{
			craftableList.emplace_back(key);
		}
	}
	return craftableList;
}

std::vector<size_t> Job::getCraftableList() const
{
	std::vector<size_t> craftableList;

	if(craftFactory->owner!=nullptr)
	{
		for (auto key : craftFactory->getKeys())
		{
			if (craftFactory->isCraftable(key))
			{
				craftableList.emplace_back(key);
			}
		}
	}
	return craftableList;
}

std::vector<size_t> Job::getUncraftableList() const
{
	std::vector<size_t> uncraftableList;

	if (craftFactory->owner != nullptr)
	{
		for (auto key : craftFactory->getKeys())
		{
			if (!craftFactory->isCraftable(key))
			{
				uncraftableList.emplace_back(key);
			}
		}
	}
	return uncraftableList;
}

[[maybe_unused]] const std::list<size_t>& Job::getUsableTools() const
{
	return usableToolsList;
}

std::list<size_t>& Job::getUsableTools()
{
	return usableToolsList;
}

Craft* Job::getCraft(const size_t key) const
{
	return craftFactory->getDefaultObject(key);
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
