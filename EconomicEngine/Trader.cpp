#include "Trader.h"
#include <chrono>
#include <memory>
#include <random>


#include "Countable.h"
#include "Food.h"
#include "StockExchange.h"
#include "TradableManager.h"
#include "TraderManager.h"
#include "Uncountable.h"

Trader::Trader()
{
	currentJob = nullptr;
	currentCraft = nullptr;
	money = 100;
	foodLevel = 20.0f;
	randomEngine = std::mt19937(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	TradableManager* tradableManager = TradableManager::getInstance();
	auto keys = tradableManager->getKeys();
	priceBeliefs = VectorArray<float>(keys);
	for (auto key : keys)
	{
		auto tradableDefaultPriceBelief = tradableManager->getTradable(key)->getDefaultPriceBelief();
		priceBeliefs[key].resize(2);
		priceBeliefs[key][0] = std::make_shared<float>(tradableDefaultPriceBelief.first);
		priceBeliefs[key][1] = std::make_shared<float>(tradableDefaultPriceBelief.second);
	}
}

void Trader::makeAsks()
{
	if (currentJob != nullptr)
	{
		//BuyingAsks
		wonderList.clear();
		fillWonderList();

		for (const auto wonderItem : wonderList)
		{
			float price = evaluatePrice(wonderItem.first);
			if (price <= money)
			{
				auto buyingAsk = std::make_shared<BuyingAsk>(wonderItem.first, wonderItem.second, price);
				StockExchange::getInstance()->registerAsk(buyingAsk);
				this->currentAsks.emplace_back(buyingAsk);
			}
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
	if (currentJob != nullptr && currentCraft == nullptr)
	{
		const auto craftableList = this->currentJob->getCraftableList();
		if (!craftableList.empty())
		{
			const std::uniform_int_distribution<int> uniformDist(0, static_cast<int>(craftableList.size() - 1));
			this->currentCraft = this->currentJob->craft(this->currentJob->getCraftableList()[uniformDist(randomEngine)]);

			for (auto& item : inventory)
			{
				auto* uncountable = dynamic_cast<Uncountable*>(item.get());
				if (uncountable != nullptr && uncountable->getBehavior() != nullptr)
				{
					for (auto usableTool : currentJob->getUsableTools())
					{
						if (usableTool == uncountable->getId())
						{
							uncountable->getBehavior()->init(this, uncountable);
							break;
						}
					}
				}
			}
			for (const auto requirement : currentCraft->getRequirement())
			{
				removeFromInventory(requirement.first, requirement.second);
			}
		}
	}
	if (currentCraft != nullptr)
	{
		const size_t result = currentCraft->advanceCraft();
		if (result != 0)
		{
			delete currentCraft;
			currentCraft = nullptr;
			addToInventory(result, 1); //TODO multicraft
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
	//Craft Requirements
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

	//Tools
	for (auto usableTool : this->currentJob->getUsableTools())
	{
		if (!isInInventory(usableTool))
		{
			wonderList.emplace_back(std::pair<size_t, int>(usableTool, 1));
		}
	}

	//Food
	if(calculateFoodStock()<=5.0f)
	{
		TradableManager* tradableManager = TradableManager::getInstance();
		std::vector < std::pair < size_t, std::pair< float, int> >> foodInfos;
		foodInfos.reserve(tradableManager->getKeys().size());
		
		for(auto key : tradableManager->getKeys())
		{
			auto* foodItem = dynamic_cast<Food*>(tradableManager->getTradable(key));
			if(foodItem!=nullptr)
			{
				foodInfos.emplace_back(std::pair<size_t, std::pair<float, int>>(key, std::pair<float, int>(foodItem->getFoodValue(), INT_MAX)));
			}
		}

		const std::uniform_real_distribution<float> uniformFloatDist(1.0f, 10.0f);
		const float foodGoal = uniformFloatDist(randomEngine);
		wonderList.splice(wonderList.end(),getRandomFoodCombination(foodInfos, foodGoal));
	}
}

void Trader::fillGoodsList()
{
	std::list<size_t> requiredItemsId;
	auto craftableList = currentJob->getCraftableList();
	for (auto key : craftableList)
	{
		auto* craftable = currentJob->getCraft(key);
		for (auto requirement : craftable->getRequirement())
		{
			requiredItemsId.push_back(requirement.first);
		}
	}

	for (auto usableTool : getCurrentJob()->getUsableTools())
	{
		requiredItemsId.push_back(usableTool);
	}

	const float foodStock = calculateFoodStock();
	if(foodStock >= 10)
	{
		std::vector<std::pair<size_t, std::pair<float, int>>> foodInfos;
		foodInfos.reserve(inventory.size());
		for (auto& item : inventory)
		{
			auto* foodItem = dynamic_cast<Food*>(item.get());
			if (foodItem != nullptr)
			{
				foodInfos.emplace_back(std::pair<size_t, std::pair<float, int>>(foodItem->getId(), std::pair<float, int>(foodItem->getFoodValue(), foodItem->getCount())));
			}
		}

		for (const auto& food : getRandomFoodCombination(foodInfos, foodLevel-10.0f))
		{
			goodsList.emplace_back(std::pair<size_t, int>(food.first, food.second));
		}
	}

	if (requiredItemsId.empty())
	{
		requiredItemsId.emplace_back(0);
	}

	for (const auto& item : inventory)
	{
		bool isRequired = false;
		for (auto requiredItemId : requiredItemsId)
		{
			if (item->getId() == requiredItemId)
			{
				isRequired = true;
				break;
			}
		}
		if (!isRequired)
		{
			auto* countable = dynamic_cast<Countable*>(item.get());
			if (countable != nullptr)
			{
				goodsList.emplace_back(countable->getId(), countable->getCount());
			}
			else
			{
				bool yetAdded = false;
				for(auto& good : goodsList)
				{
					if(good.first == item->getId())
					{
						++good.second;
						yetAdded = true;
						break;
					}			
				}
				if (!yetAdded)
				{
					goodsList.emplace_back(item->getId(), 1);
				}

			}

		}
	}
}

std::list<std::pair<size_t, int>> Trader::getRandomFoodCombination(const std::vector<std::pair<size_t, std::pair<float, int>>>& foodInfos, const float foodGoal)
{
	std::list<std::pair<size_t, int>> foodCombination;
	if(!foodInfos.empty())
	{
		float foodCount = 0.0f;
		while (foodCount < foodGoal)
		{
			std::uniform_int_distribution<int> uniformIntDist(0, static_cast<int>(foodInfos.size() - 1));
			const int index = uniformIntDist(randomEngine);
			uniformIntDist = std::uniform_int_distribution<int>(1, std::max<int>(1, std::min<int>(foodInfos[index].second.second, static_cast<int>(foodGoal / foodInfos[index].second.first))));
			const int count = uniformIntDist(randomEngine);
			foodCount += foodInfos[index].second.first * static_cast<float>(count);
			foodCombination.emplace_back(foodInfos[index].first, count);
		}
	}

	return foodCombination;
}

float Trader::calculateFoodStock()
{
	float foodStock = 0.0f;
	for(const auto& item : inventory)
	{
		auto* foodItem = dynamic_cast<Food*>(item.get());
		if(foodItem!=nullptr)
		{
			foodStock += foodItem->getFoodValue();
		}
	}
	return foodStock;
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
	for (const auto requirement : craft->getRequirement())
	{
		requirementsPrice += calculatePriceBeliefMean(requirement.first) * requirement.second;
	}

	float advancement = 0.0f;
	int turnNumber = 0;
	while (advancement < 1.0f)
	{
		advancement += craft->getRate();
		++turnNumber;
	}

	return (resultPrice - requirementsPrice) / static_cast<float>(turnNumber);
}

void Trader::checkAsks()
{
	//TODO Re-evaluate
	for (auto& ask : currentAsks)
	{
		if (ask->getStatus() == AskStatus::Sold)
		{
			auto* buyingAsk = dynamic_cast<BuyingAsk*>(ask.get());
			if (buyingAsk != nullptr)
			{
				addToInventory(buyingAsk->getId(), buyingAsk->getCount());
				money -= ask->getPrice() * ask->getCount();
			}
			else
			{
				auto* sellingAsk = dynamic_cast<SellingAsk*>(ask.get());
				removeFromInventory(ask->getId(), sellingAsk->getTradedCount());
				money += ask->getPrice() * static_cast<float>(sellingAsk->getTradedCount());
			}
		}
	}
	currentAsks.clear();
}

void Trader::refresh()
{
	//TODO price belief, jobRanking
	if (currentJob == nullptr)
	{
		this->assignJob();
	}
	checkAsks();

	foodLevel -= 0.34f; //TODO make it a setting
	
	if(foodLevel > 0.0 && foodLevel<=10.0f)
	{
		std::vector<std::pair<size_t, std::pair<float, int>>> foodInfos;
		foodInfos.reserve(inventory.size());
		for (auto& item : inventory)
		{
			auto* foodItem = dynamic_cast<Food*>(item.get());
			if (foodItem != nullptr)
			{
				foodInfos.emplace_back(std::pair<size_t, std::pair<float, int>>(foodItem->getId(), std::pair<float, int>(foodItem->getFoodValue(), foodItem->getCount())));
			}
		}

		for(const auto& food : getRandomFoodCombination(foodInfos, 10.0f-foodLevel))
		{
			foodLevel += dynamic_cast<Food*>(TradableManager::getInstance()->getTradable(food.first))->getFoodValue() * static_cast<float>(food.second);
			removeFromInventory(food.first, food.second);
		}
	}
}

const std::list<std::shared_ptr<Tradable>>& Trader::getInventory() const
{
	return inventory;
}

Job* Trader::getCurrentJob() const
{
	return this->currentJob;
}

Craft* Trader::getCurrentCraft() const
{
	return this->currentCraft;
}

bool Trader::isInInventory(const size_t key)
{
	for (auto& item : inventory)
	{
		if (item->getId() == key)
		{
			return true;
		}
	}
	return false;
}

float Trader::getFoodLevel() const
{
	return foodLevel;
}

void Trader::addToInventory(const size_t key, const int count)
{
	auto* tradableManager = TradableManager::getInstance();

	auto* item = tradableManager->createTradable(key);
	auto* countable = dynamic_cast<Countable*>(item);
	if (countable != nullptr)
	{
		countable->setCount(count);
		addToInventory(countable);
	}
	else
	{
		auto* uncountable = dynamic_cast<Uncountable*>(item);
		addToInventory(uncountable);
		for (auto i = 0; i < count - 1; ++i)
		{
			uncountable = dynamic_cast<Uncountable*>(tradableManager->createTradable(key));
			addToInventory(uncountable);
		}
	}
}

void Trader::addToInventory(Countable* countable)
{
	for (auto& item : inventory)
	{
		if (item->getId() == countable->getId())
		{
			dynamic_cast<Countable*>(item.get())->incrementCountBy(countable->getCount());
			return;
		}
	}
	inventory.emplace_back(countable);
}

void Trader::addToInventory(Uncountable* uncountable)
{
	inventory.emplace_back(uncountable);
}

void Trader::removeFromInventory(const size_t key, const int count = 1)
{
	for (auto& item : inventory)
	{
		if (item->getId() == key)
		{
			auto* countable = dynamic_cast<Countable*>(item.get());
			if (countable != nullptr)
			{
				countable->decrementCountBy(count);
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

void Trader::removeFromInventory(Tradable* tradable)
{
	for (auto& item : inventory)
	{
		if (item.get() == tradable)
		{
			item.reset();
			inventory.remove(item);
			break;
		}
	}
}
