#include "Trader.h"
#include <chrono>
#include <memory>
#include <random>

#include "TraderManager.h"

Trader::Trader()
{
	currentJob = nullptr;
	currentCraft = nullptr;
	randomEngine = std::mt19937(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

void Trader::makeAsks()
{
	
	const std::uniform_int_distribution<int> uniformDist(0, 1);
	for(auto tradable : wonderList)
	{	
		if(uniformDist(randomEngine))
		{
			//TODO
		}
	}

	for (auto tradable : goodsList)
	{
		if (uniformDist(randomEngine))
		{
			//TODO
		}
	}
}

void Trader::craft()
{
	//TODO use wonderlist
	if(currentJob!=nullptr && currentCraft==nullptr)
	{
		const auto craftableList = this->currentJob->getCraftableList();
		if(!craftableList.empty())
		{
			const std::uniform_int_distribution<int> uniformDist(0, static_cast<int>(craftableList.size()-1));
			this->currentCraft = this->currentJob->craft(this->currentJob->getCraftableList()[uniformDist(randomEngine)]);
		}
			
	}
	else if(currentCraft!=nullptr)
	{
		Tradable* result = currentCraft->advanceCraft();
		if(result != nullptr)
		{
			delete currentCraft;
			currentCraft = nullptr;
			addToInventory(result);
		}
	}
	
}

void Trader::refresh()
{
	//TODO price belief, wonderlist, goodslist, jobRanking
	if(currentJob == nullptr)
	{
		this->assignJob();
	}
}

void Trader::assignJob()
{
	TraderManager* traderManager = TraderManager::getInstance();
	this->currentJob = traderManager->assignJob(traderManager->getMostInterestingJob(), this);
}

const std::list<std::shared_ptr<Tradable>>& Trader::getInventory() const
{
	return inventory;
}

const Job* Trader::getCurrentJob() const
{
	return this->currentJob;
}
