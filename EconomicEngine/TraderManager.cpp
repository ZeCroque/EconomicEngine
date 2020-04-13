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

std::list<std::pair<size_t, std::string>> TraderManager::getJobList() const
{
	std::list<std::pair<size_t, std::string>> result;
	for(const auto& job : jobFactory.getJobs())
	{
		result.emplace_back(std::pair<size_t, std::string>(job->getId(), job->getName()));
	}
	return result;
}

int TraderManager::getJobCount(const size_t key)
{
	int count = 0;
	for (const auto& trader : this->traders)
	{
		if (trader.getCurrentJob() != nullptr)
		{
			if(trader.getCurrentJob()->getId() == key)
			{
				++count;
			}
		}
	}
	return count;
}

size_t TraderManager::getMostInterestingJob()
{
	std::list<std::pair<size_t, int>> jobCounts;
	for(auto key : jobFactory.getKeys())
	{
		jobCounts.emplace_back(std::pair<size_t, int>(key, getJobCount(key)));
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

void TraderManager::kill(const size_t key, int count)
{
	std::vector<std::list<Trader>::iterator> iterators;
	iterators.reserve(traders.size());

	int i = 0;
	for(auto  it = traders.begin(); it!=traders.end() && i<count; ++it)
	{
		if(it->getCurrentJob()->getId() == key)
		{
			iterators.emplace_back(it);
			++i;
		}
	}

	for(auto& it : iterators)
	{
		traders.erase(it);
	}
}
