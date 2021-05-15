#include <algorithm>

#include "Traders/TraderManager.h"
#include "Traders/Trader.h"

void TraderManager::init() const
{
	const auto& jobsIds = jobFactory.getKeys();
	demographyCounts = VectorArray <std::pair<int, int>>(jobsIds);
	for(const auto& jobId : jobsIds)
	{
		demographyCounts[jobId].emplace_back(std::make_shared <std::pair<int, int>>(0, 0));
	}
}

void TraderManager::reset()
{
	traders.clear();
	for(const auto& jobId : jobFactory.getKeys())
	{
		demographyCounts[jobId].front()->first = 0;
		demographyCounts[jobId].front()->second = 0;
	}
}

void TraderManager::update(const float inDeltaTime)
{
	for (auto& trader : traders)
	{
		trader.update(inDeltaTime);
	}
}

void TraderManager::registerJob(Job* inJob)
{
	jobFactory.registerObject(inJob->getId(), inJob);
}

void TraderManager::addTraders(const int inAmount)
{
	for (int i = 0; i < inAmount; ++i)
	{	
		traders.emplace_back(Trader(jobFactory.createObject(getMostInterestingJobId())));
		traders.back().getCurrentJob()->setOwner(&traders.back());
        traderAddedSignal(&traders.back());
	}
}

void TraderManager::addTraders(const int inAmount, const size_t jobId)
{
	for (int i = 0; i < inAmount; ++i)
	{
		traders.emplace_back(Trader(jobFactory.createObject(jobId)));
		traders.back().getCurrentJob()->setOwner(&traders.back());
		++demographyCounts[traders.back().getCurrentJob()->getId()][0]->first;
        traderAddedSignal(&traders.back());
	}	
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

void TraderManager::markForKill(const size_t inJobId, const int inAmount)
{
	for(int i = 0, deletedCount = 0; auto& trader : traders)
	{
		if(deletedCount == inAmount)
		{
			break;
		}
		
		if(trader.getCurrentJob()->getId() == inJobId)
		{
			pendingKillTraders.push_back(&trader);
			++deletedCount;
		}
		++i;
	}
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

std::list<std::pair<size_t, std::string>> TraderManager::getJobsIds() const
{
	std::list<std::pair<size_t, std::string>> result;
	for(const auto& job : jobFactory.getJobsDefaultObjects())
	{
		result.emplace_back(std::pair<size_t, std::string>(job->getId(), job->getName()));
	}
	return result;
}

std::pair<int, int> TraderManager::getDemographyByJob(const size_t inJobId) const
{
	return *demographyCounts[inJobId].front();
}

std::list<const Trader*> TraderManager::getTraderByJob(const size_t inJobId) const
{
	std::list<const Trader*> traderList;
	for (const auto& trader : traders)
	{
		if (trader.getCurrentJob() && trader.getCurrentJob()->getId() == inJobId)
		{	
			traderList.push_back(&trader);		
		}
	}
	return traderList;
}

float TraderManager::getMoneyMeanByJob(const size_t inJobId) const
{
	float total = 0.f;
	const auto& tradersByJobId = getTraderByJob(inJobId);
	for(const auto& trader : tradersByJobId)
	{
		total += trader->getMoneyCount();
	}
	return total / static_cast<float>(std::max<size_t>(1, tradersByJobId.size()));
}

float TraderManager::getFoodLevelMeanByJob(const size_t inJobId) const
{
	float total = 0.f;
	const auto& tradersByJobId = getTraderByJob(inJobId);
	for (const auto& trader : tradersByJobId)
	{
		total += trader->getFoodLevel();
	}
	return total / static_cast<float>(std::max<size_t>(1, tradersByJobId.size()));
}

int TraderManager::getTraderCountByJob(const size_t inJobId) const
{
	return static_cast<int>(getTraderByJob(inJobId).size());
}

size_t TraderManager::getMostInterestingJobId() const
{
	std::list<std::pair<size_t, int>> jobCounts;
	for(auto jobId : jobFactory.getKeys())
	{
		jobCounts.emplace_back(std::pair<size_t, int>(jobId, getTraderCountByJob(jobId)));
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

const Signal <Trader*> &TraderManager::getTraderAddedSignal() const
{
    return traderAddedSignal;
}
