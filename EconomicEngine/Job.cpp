#include "Job.h"

#include "Countable.h" //DEBUG

Job::Job() : craftFactory(new CraftFactory()){}

Job::Job(const Job& job) : Job()
{
	this->craftFactory = job.craftFactory->clone();
}

Job::~Job()
{
	delete craftFactory;
}

void Job::setOwner(Trader* owner) const
{
	this->craftFactory->setOwner(owner);
}

Craft* Job::craft(const size_t typeId) const
{
	return this->craftFactory->createObject(typeId);
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

Craft* Job::getCraft(const size_t key) const
{
	return craftFactory->getDefaultObject(key);
}

//DEBUG

Miner::Miner()
{
	//Gold : no requirement
	this->craftFactory->registerCraft(new Craft(0.1f, typeid(Gold).hash_code(), std::vector<std::pair<size_t, int>>()));
}

Miner* Miner::clone()
{
	return new Miner(*this);
}
