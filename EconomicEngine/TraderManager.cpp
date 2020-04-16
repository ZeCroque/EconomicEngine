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

void TraderManager::addTrader(const int count, const size_t key)
{
	for (int i = 0; i < count; ++i)
	{
		traders.emplace_back(Trader(jobFactory.createObject(key)));
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

std::list<Trader*> TraderManager::getTraderByJobId(const size_t key)
{
	std::list<Trader*> traderList;
	for (auto& trader : this->traders)
	{
		if (trader.getCurrentJob() != nullptr)
		{
			if (trader.getCurrentJob()->getId() == key)
			{
				traderList.emplace_back(&trader);
			}
		}
	}
	return traderList;
}

float TraderManager::getMoneyMeanByJob(const size_t key)
{
	float total = 0.0f;
	const auto& traders = getTraderByJobId(key);
	for(const auto& trader : traders)
	{
		total += trader->getMoney();
	}
	return total / static_cast<float>(std::max<size_t>(1, traders.size()));
}

int TraderManager::getJobCount(const size_t key)
{
	return static_cast<int>(getTraderByJobId(key).size());
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

void TraderManager::killTraders()
{
	std::vector<std::list<Trader>::iterator> iterators;
	iterators.reserve(traders.size());

	for (auto it = traders.begin(); it != traders.end(); ++it)
	{
		if (it->getFoodLevel() <= 0)
		{
			iterators.emplace_back(it);
		}
	}

	for (auto& it : iterators)
	{
		traders.erase(it);
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

void TraderManager::kill(const size_t key, const int count)
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
