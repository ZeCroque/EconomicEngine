#include "Traders/TraderManager.h"
#include <algorithm>

#include "Traders/Trader.h"

void TraderManager::init() const
{
	const auto& keys = jobFactory.getKeys();
	demographyCounts = VectorArray <std::pair<int, int>>(keys);
	for(const auto& key : keys)
	{
		demographyCounts[key].emplace_back(std::make_shared <std::pair<int, int>>(0, 0));
	}
}

void TraderManager::registerJob(Job* job)
{
	jobFactory.registerObject(job->getId(), job);
}

void TraderManager::addTrader(const int count)
{
	for (int i = 0; i < count; ++i)
	{	
		traders.emplace_back(Trader(jobFactory.createObject(getMostInterestingJob())));
		traders.back().getCurrentJob()->setOwner(&traders.back());
        traderAddedSignal(&traders.back());
	}
}

void TraderManager::addTrader(const int count, const size_t key)
{
	for (int i = 0; i < count; ++i)
	{
		traders.emplace_back(Trader(jobFactory.createObject(key)));
		traders.back().getCurrentJob()->setOwner(&traders.back());
		++demographyCounts[traders.back().getCurrentJob()->getId()][0]->first;
        traderAddedSignal(&traders.back());
	}	
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

std::pair<int, int> TraderManager::getDemographyByJob(const size_t key) const
{
	return *demographyCounts[key].front();
}

std::list<const Trader*> TraderManager::getTraderByJobId(const size_t key) const
{
	std::list<const Trader*> traderList;
	for (const auto& trader : traders)
	{
		if (trader.getCurrentJob() != nullptr && trader.getCurrentJob()->getId() == key)
		{	
			traderList.push_back(&trader);		
		}
	}
	return traderList;
}

float TraderManager::getMoneyMeanByJob(const size_t key) const
{
	float total = 0.0f;
	const auto& tradersByJobId = getTraderByJobId(key);
	for(const auto& trader : tradersByJobId)
	{
		total += trader->getMoney();
	}
	return total / static_cast<float>(std::max<size_t>(1, tradersByJobId.size()));
}

float TraderManager::getFoodLevelMeanByJob(const size_t key) const
{
	float total = 0.0f;
	const auto& tradersByJobId = getTraderByJobId(key);
	for (const auto& trader : tradersByJobId)
	{
		total += trader->getFoodLevel();
	}
	return total / static_cast<float>(std::max<size_t>(1, tradersByJobId.size()));
}

int TraderManager::getJobCount(const size_t key) const
{
	return static_cast<int>(getTraderByJobId(key).size());
}

size_t TraderManager::getMostInterestingJob() const
{
	std::list<std::pair<size_t, int>> jobCounts;
	for(auto key : jobFactory.getKeys())
	{
		jobCounts.emplace_back(std::pair<size_t, int>(key, getJobCount(key)));
	}

	std::pair<size_t, int> minCount(0,INT_MAX);
	for(const auto& jobCount : jobCounts)
	{
		if(jobCount.second < minCount.second)
		{
			minCount = jobCount;
		}
	}

	return minCount.first;
}

void TraderManager::killStarvedTraders()
{
	traders.remove_if([this](const Trader& trader)
	{
		if(trader.getFoodLevel() <= 0)
		{
            trader.getDeathSignal()();
            ++demographyCounts[trader.getCurrentJob()->getId()][0]->second;
			return true;
		}
		return false;
	});
}

void TraderManager::clearPendingKillTraders()
{
	traders.remove_if([this](const Trader& trader)
	{
		for(const auto* pendingKillTrader : pendingKillTraders)
		{
			if(&trader == pendingKillTrader)
			{
			    trader.getDeathSignal()();
                ++demographyCounts[trader.getCurrentJob()->getId()][0]->second;
				return true;
			}
		}
		return false;
	});
	pendingKillTraders.clear();
}

void TraderManager::makeChildren()
{
	for (auto& trader : traders)
	{
		trader.makeChild();
	}
}

void TraderManager::update(float deltaTime)
{
	for (auto& trader : traders)
	{
		trader.update(deltaTime);
	}
}

void TraderManager::reset()
{
	traders.clear();
	for(const auto& key : jobFactory.getKeys())
	{
		demographyCounts[key].front()->first = 0;
		demographyCounts[key].front()->second = 0;
	}
}

void TraderManager::markForKill(const size_t key, const int count)
{
	int i = 0;
	int deletedCount = 0;
	for(auto& trader : traders)
	{
		if(deletedCount == count)
		{
			break;
		}
		
		if(trader.getCurrentJob()->getId() == key)
		{
			pendingKillTraders.push_back(&trader);
			++deletedCount;
		}
		++i;
	}
}

const Signal <Trader*> &TraderManager::getTraderAddedSignal() const
{
    return traderAddedSignal;
}
