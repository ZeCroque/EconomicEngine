#include "TraderManager.h"

void TraderManager::registerJob(Job* job)
{
	this->jobFactory.registerObject(typeid(*job).hash_code(), job);
}

void TraderManager::addTrader(const int count)
{
	for (int i = 0; i < count; ++i)
	{
		this->traders.emplace_back(Trader());
	}
}

Job* TraderManager::assignJob(const size_t key, Trader* trader) const
{
	Job* job = this->jobFactory.createObject(key);
	job->setOwner(trader);
	return job;
}

size_t TraderManager::getMostInterestingJob()
{
	std::map<size_t, int> jobCounts;
	for(auto key : this->jobFactory.getKeys())
	{
		jobCounts.emplace(std::pair<size_t, int>(key, 0));
	}
	
	for(const auto& trader : this->traders)
	{
		if(trader.getCurrentJob() != nullptr)
		{
			++jobCounts.find(typeid(*trader.getCurrentJob()).hash_code())->second;
		}
	}

	std::pair<size_t, int> minCount(0,INT_MAX);
	for(const auto jobCount : jobCounts)
	{
		if(jobCount.second < minCount.second)
		{
			minCount = jobCount;
		}
	}

	return minCount.first;
}

void TraderManager::refreshTraders()
{
	for(auto& trader : traders)
	{
		trader.refresh();
	}
}

void TraderManager::doTradersCrafting()
{
	for (auto& trader : traders)
	{
		trader.craft();
	}
}

void TraderManager::doTradersAsking()
{
	for (auto& trader : traders)
	{
		trader.makeAsks();
	}
}

void TraderManager::reset()
{
	traders.clear();
}
