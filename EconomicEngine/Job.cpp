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
Farmer::Farmer() : Job()
{
	//Wheat : no requirement
	this->craftFactory->registerCraft(new Craft(0.5f, typeid(Wheat).hash_code(), std::vector<std::pair<size_t, int>>()));


	std::vector<std::pair<size_t, int>> requirements;

	//Bread : require one wheat
	requirements.emplace_back(std::pair<size_t, int>(typeid(Wheat).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(0.34f, typeid(Bread).hash_code(), requirements));

	//GoldenBread requires one bread one gold
	requirements.clear();
	requirements.emplace_back(std::pair<size_t, int>(typeid(Bread).hash_code(), 1));
	requirements.emplace_back(std::pair<size_t, int>(typeid(Gold).hash_code(), 1));
	this->craftFactory->registerCraft(new Craft(1.0f, typeid(GoldenBread).hash_code(), requirements));
	
}