#include "Job.h"

#include "Countable.h" //DEBUG
#include "Uncountable.h"

Job::Job() : craftFactory(new CraftFactory()){}

Job::Job(const Job& job) : Job()
{
	this->craftFactory = job.craftFactory->clone();
	this->usableToolsList = std::list<size_t>(job.usableToolsList);
	this->id = job.id;
}

Job::~Job()
{
	delete craftFactory;
}

void Job::setOwner(Trader* owner) const
{
	this->craftFactory->setOwner(owner);
}

Craft* Job::createCraft(const size_t typeId) const
{
	return this->craftFactory->createObject(typeId);
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

std::list<size_t> Job::getUsableTools() const
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
