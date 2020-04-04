#include "Job.h"


Job::Job() : craftFactory(nullptr){}

Job::Job(Trader& owner) : craftFactory(new CraftFactory(owner)){}

Job::~Job()
{
	delete craftFactory;
}

Craft* Job::craft(const size_t typeId) const
{
	return this->craftFactory->createObject(typeId);
}

std::vector<size_t> Job::getCraftableList() const
{
	std::vector<size_t> craftableList;
	for(auto key : craftFactory->getKeys())
	{
		if(craftFactory->isCraftable(key))
		{
			craftableList.emplace_back(key);
		}
	}

	return craftableList;
}