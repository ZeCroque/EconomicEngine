#include "Trader.h"
#include <chrono>
#include <memory>
#include <random>



#include "Countable.h"
#include "StockExchange.h"
#include "TradableManager.h"
#include "TraderManager.h"

Trader::Trader()
{
	currentJob = nullptr;
	currentCraft = nullptr;
	money = 0;
	randomEngine = std::mt19937(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	TradableManager* tradableManager = TradableManager::getInstance();
	auto keys = tradableManager->getKeys();
	priceBeliefs = VectorArray<float>(keys);
	for(auto key : keys)
	{
		auto tradableDefaultPriceBelief = tradableManager->getTradable(key)->getDefaultPriceBelief();
		priceBeliefs[key].resize(2);
		priceBeliefs[key][0] = std::make_shared<float>(tradableDefaultPriceBelief.first);
		priceBeliefs[key][1] = std::make_shared<float>(tradableDefaultPriceBelief.second);
	}
	
}

void Trader::makeAsks()
{
	if(currentJob != nullptr)
	{	
		//BuyingAsks
		wonderList.clear();
		fillWonderList();
			
		for(const auto wonderItem : wonderList)
		{
			auto buyingAsk = std::make_shared<BuyingAsk>(wonderItem.first, wonderItem.second, evaluatePrice(wonderItem.first));
			StockExchange::getInstance()->registerAsk(buyingAsk);
			this->currentAsks.emplace_back(buyingAsk);
		}
		
		//SellingAsks
		goodsList.clear();
		fillGoodsList();
		
		for (const auto goodItem : goodsList)
		{
			auto sellingAsk = std::make_shared<SellingAsk>(goodItem.first, goodItem.second, evaluatePrice(goodItem.first));
			StockExchange::getInstance()->registerAsk(sellingAsk);
			this->currentAsks.emplace_back(sellingAsk);
		}
	}
}

void Trader::craft()
{
	if(currentJob!=nullptr && currentCraft==nullptr)
	{
		const auto craftableList = this->currentJob->getCraftableList();
		if(!craftableList.empty())
		{
			const std::uniform_int_distribution<int> uniformDist(0, static_cast<int>(craftableList.size()-1));
			this->currentCraft = this->currentJob->craft(this->currentJob->getCraftableList()[uniformDist(randomEngine)]);
			for(const auto requirement : currentCraft->getRequirement())
			{
				removeFromInventory(requirement.first);
			}
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

void Trader::assignJob()
{
	TraderManager* traderManager = TraderManager::getInstance();
	this->currentJob = traderManager->assignJob(traderManager->getMostInterestingJob(), this);
}

void Trader::fillWonderList()
{
	Craft* mostBeneficialCraft = nullptr;

	auto uncraftableList = currentJob->getUncraftableList();
	for (auto key : uncraftableList)
	{
		auto* craftable = currentJob->getCraft(key);
		if (mostBeneficialCraft == nullptr || calculateEarnings(craftable) > calculateEarnings(mostBeneficialCraft))
		{
			mostBeneficialCraft = craftable;
		}
	}

	if (mostBeneficialCraft != nullptr)
	{
		for (auto requirement : mostBeneficialCraft->getRequirement())
		{
			if (this->getCurrentJob()->getCraft(requirement.first) == nullptr)
			{
				wonderList.emplace_back(requirement);
			}
		}
	}
}

void Trader::fillGoodsList()
{

	std::list<size_t> requiredItemsId(1);
	auto craftableList = currentJob->getCraftableList();
	for (auto key : craftableList)
	{
		auto* craftable = currentJob->getCraft(key);
		for(auto requirement : craftable->getRequirement())
		{
			requiredItemsId.push_back(requirement.first);
		}
	}

	for(const auto& item : inventory)
	{
		for(auto requiredItemId : requiredItemsId)
		{
			if (item->getId() != requiredItemId)
			{
				goodsList.emplace_back(item->getId(),1);
			}
		}		
	}
}

float Trader::calculatePriceBeliefMean(const size_t key)
{
	auto resultPriceBelief = priceBeliefs[key];
	return (*resultPriceBelief.front() + *resultPriceBelief.back()) / 2.0f;
}

float Trader::evaluatePrice(const size_t key)
{
	const std::uniform_int_distribution<int> uniformIntDist(0, 1);
	std::uniform_real_distribution<float> uniformFloatDist;
	const float mean = calculatePriceBeliefMean(key);
	if (uniformIntDist(randomEngine))
	{
		uniformFloatDist = std::uniform_real_distribution<float>(*priceBeliefs[key].front(), mean);
	}
	else
	{
		uniformFloatDist = std::uniform_real_distribution<float>(mean, *priceBeliefs[key].back());
	}
	return uniformFloatDist(randomEngine);
	
}

float Trader::calculateEarnings(Craft* craft)
{
	const float resultPrice = calculatePriceBeliefMean(craft->getResult());

	float requirementsPrice = 0;
	for(const auto requirement : craft->getRequirement())
	{
		requirementsPrice += calculatePriceBeliefMean(requirement.first) * requirement.second;
	}

	float advancement = 0.0f;
	int turnNumber = 0;
	while(advancement < 1.0f)
	{
		advancement += craft->getRate();
		++turnNumber;
	}

	return (resultPrice - requirementsPrice) / static_cast<float>(turnNumber);
}

void Trader::checkAsks()
{
	for(auto& ask : currentAsks)
	{
		if(ask->getStatus()==AskStatus::Sold)
		{
			auto* buyingAsk = dynamic_cast<BuyingAsk*>(ask.get());
			if(buyingAsk != nullptr)
			{
				//TODO remove money
				addToInventory(TradableManager::getInstance()->createTradable(buyingAsk->getResult().first)); //TODO count
			}
			else
			{
				money += ask->getPrice()*ask->getCount();
				int z = 8;
			}
		}
	}

	int i = 42;
}

void Trader::refresh()
{
	//TODO price belief, wonderlist, goodslist, jobRanking
	if(currentJob == nullptr)
	{
		this->assignJob();
	}

	checkAsks();
}

const std::list<std::shared_ptr<Tradable>>& Trader::getInventory() const
{
	return inventory;
}

const Job* Trader::getCurrentJob() const
{
	return this->currentJob;
}

void Trader::addToInventory(Tradable* tradable)
{
	auto* countable = dynamic_cast<Countable*>(tradable);
	if (countable != nullptr)
	{
		for (auto& item : inventory)
		{
			if (item->getId() == tradable->getId())
			{
				dynamic_cast<Countable*>(item.get())->incrementCountBy(1);
				return;
			}
		}
	}
	inventory.emplace_back(tradable);
}

void Trader::removeFromInventory(const size_t key)
{
	for (auto& item : inventory)
	{
		if (item->getId() == key)
		{
			auto* countable = dynamic_cast<Countable*>(item.get());
			if (countable != nullptr)
			{
				countable->decrementCountBy(1);
				if (countable->getCount() == 0)
				{
					item.reset();
					inventory.remove(item);
				}
				return;
			}
			item.reset();
			inventory.remove(item);
			return;
		}
	}
}
