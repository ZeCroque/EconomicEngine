#include <algorithm>

#include "Traders/TraderManager.h"
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

void TraderManager::registerJob(Job* inJob)
{
	jobFactory.registerObject(inJob->getId(), inJob);
}

void TraderManager::addTrader(const int inCount)
{
	for (int i = 0; i < inCount; ++i)
	{	
		traders.emplace_back(Trader(jobFactory.createObject(getMostInterestingJob())));
		traders.back().getCurrentJob()->setOwner(&traders.back());
        traderAddedSignal(&traders.back());
	}
}

void TraderManager::addTrader(const int inCount, const size_t inKey)
{
	for (int i = 0; i < inCount; ++i)
	{
		traders.emplace_back(Trader(jobFactory.createObject(inKey)));
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

std::pair<int, int> TraderManager::getDemographyByJob(const size_t inKey) const
{
	return *demographyCounts[inKey].front();
}

std::list<const Trader*> TraderManager::getTraderByJobId(const size_t inKey) const
{
	std::list<const Trader*> traderList;
	for (const auto& trader : traders)
	{
		if (trader.getCurrentJob() && trader.getCurrentJob()->getId() == inKey)
		{	
			traderList.push_back(&trader);		
		}
	}
	return traderList;
}

float TraderManager::getMoneyMeanByJob(const size_t inKey) const
{
	float total = 0.0f;
	const auto& tradersByJobId = getTraderByJobId(inKey);
	for(const auto& trader : tradersByJobId)
	{
		total += trader->getMoney();
	}
	return total / static_cast<float>(std::max<size_t>(1, tradersByJobId.size()));
}

float TraderManager::getFoodLevelMeanByJob(const size_t inKey) const
{
	float total = 0.0f;
	const auto& tradersByJobId = getTraderByJobId(inKey);
	for (const auto& trader : tradersByJobId)
	{
		total += trader->getFoodLevel();
	}
	return total / static_cast<float>(std::max<size_t>(1, tradersByJobId.size()));
}

int TraderManager::getJobCount(const size_t inKey) const
{
	return static_cast<int>(getTraderByJobId(inKey).size());
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

void TraderManager::update(const float inDeltaTime)
{
	for (auto& trader : traders)
	{
		trader.update(inDeltaTime);
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

void TraderManager::markForKill(const size_t inKey, const int inCount)
{
	for(int i = 0, deletedCount = 0; auto& trader : traders)
	{
		if(deletedCount == inCount)
		{
			break;
		}
		
		if(trader.getCurrentJob()->getId() == inKey)
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
