#include "Job.h"


Job::Job() : craftFactory(nullptr){}

Job::Job(const Job& job) : Job()
{
	if(job.craftFactory!=nullptr)
	{
		this->craftFactory = new CraftFactory(*job.craftFactory);
	}
}

Job::~Job()
{
	delete craftFactory;
}

void Job::setOwner(Trader* owner)
{
	this->craftFactory = new CraftFactory(owner);
}

Craft* Job::craft(const size_t typeId) const
{
	Craft* craft = nullptr;
	if(craftFactory != nullptr)
	{
		craft = this->craftFactory->createObject(typeId);
	}
	return craft;
}

std::vector<size_t> Job::getCraftableList() const
{
	std::vector<size_t> craftableList;

	if(craftFactory!=nullptr)
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

Job* Job::clone()
{
	return new Job(*this);
}

void Job::init() {}
