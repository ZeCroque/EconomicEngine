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
	this->jobFactory.registerObject(job->getId(), job);
}

void TraderManager::addTrader(const int count)
{
	for (int i = 0; i < count; ++i)
	{	
		this->traders.emplace_back(Trader(jobFactory.createObject(getMostInterestingJob())));
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

float TraderManager::getMoneyMeanByJob(const size_t key) const
{
	float total = 0.0f;
	const auto& NewTraders = getTraderByJobId(key);
	for(const auto& trader : NewTraders)
	{
		total += trader->getMoney();
	}
	return total / static_cast<float>(std::max<size_t>(1, NewTraders.size()));
}

float TraderManager::getFoodLevelMeanByJob(const size_t key) const
{
	float total = 0.0f;
	const auto& NewTraders = getTraderByJobId(key);
	for (const auto& trader : NewTraders)
	{
		total += trader->getFoodLevel();
	}
	return total / static_cast<float>(std::max<size_t>(1, NewTraders.size()));
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
	for(const auto jobCount : jobCounts)
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
	std::vector<std::list<Trader>::iterator> iterators;
	iterators.reserve(traders.size());

	for (auto it = traders.begin(); it != traders.end(); ++it)
	{
		it->updateFoodLevel();
		if (it->getFoodLevel() <= 0)
		{		
			iterators.emplace_back(it);
		}
	}

	for (auto& it : iterators)
	{
		++demographyCounts[it->getCurrentJob()->getId()][0]->second;
		traders.erase(it);
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

void TraderManager::kill(const size_t key, const int count)
{
	std::list<std::list<Trader>::iterator> iterators;

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

const Signal <Trader*> &TraderManager::getTraderAddedSignal() const {
    return traderAddedSignal;
}

const Signal<Trader *> &TraderManager::getTraderKilledSignal() const {
    return traderKilledSignal;
}
