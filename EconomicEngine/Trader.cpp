#include "Trader.h"
#include <chrono>
#include <memory>
#include <random>



#include "../EconomicEngineDebugGUI/Steak.h"
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
	foodLevel = 30.0f;
	randomEngine = std::mt19937(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	TradableManager* tradableManager = TradableManager::getInstance();
	auto keys = tradableManager->getKeys();
	priceBeliefs = VectorArray<float>(keys);
	priceHistory = VectorArray<std::pair<float, int>>(keys);
	failCount = VectorArray<int>(keys);
	for (auto key : keys)
	{
		auto tradableDefaultPriceBelief = tradableManager->getTradable(key)->getDefaultPriceBelief();
		priceBeliefs[key].resize(2);
		priceBeliefs[key][0] = std::make_shared<float>(tradableDefaultPriceBelief.first);
		priceBeliefs[key][1] = std::make_shared<float>(tradableDefaultPriceBelief.second);
		priceHistory[key].emplace_back(std::make_shared<std::pair<float, int>>(calculatePriceBeliefMean(key), 1));
		failCount[key].emplace_back(std::make_shared<int>(0));
	}
}

Trader::Trader(Job* job) : Trader()
{
	currentJob = job;
}


//Auto-assign job for the trader
void Trader::assignJob()
{
	TraderManager* traderManager = TraderManager::getInstance();
	this->currentJob = traderManager->assignJob(traderManager->getMostInterestingJob(), this);
}

//Return a random combination of food to satisfy the provided foodGoal, according to the limits specified in provided foodInfos
std::list<std::pair<size_t, int>> Trader::getRandomFoodCombination(std::vector<std::pair<size_t, std::pair<float, int>>>& foodInfos, const float foodGoal)
{
	std::list<std::pair<size_t, int>> foodCombinations;

	float foodCount = 0.0f;
	while (foodCount < foodGoal && !foodInfos.empty())
	{
		//Picks a food randomly in foodInfos
		std::uniform_int_distribution<int> uniformIntDist(0, static_cast<int>(foodInfos.size() - 1));
		const int index = uniformIntDist(randomEngine);

		//Establish the food count, that is random between 1 and either the number of items necessary to fulfill the food goal or the max count of food provided in entry table
		uniformIntDist = std::uniform_int_distribution<int>(1, std::max<int>(1, std::min<int>(foodInfos[index].second.second, std::roundf(foodGoal / foodInfos[index].second.first))));
		const int count = uniformIntDist(randomEngine);

		//Decrease food count of picked item
		foodInfos[index].second.second-=count;

		//Update the foodCount from the provided data
		foodCount += foodInfos[index].second.first * static_cast<float>(count);

		//Then checks if the item we picked were previously picked or not, increment it or adds it to combination accordingly
		bool yetAdded = false;
		for(auto& foodCombination : foodCombinations)
		{
			if(foodCombination.first == foodInfos[index].first)
			{
				yetAdded = true;
				++foodCombination.second;
			}
		}
		if(!yetAdded)
		{
			foodCombinations.emplace_back(foodInfos[index].first, count);
		}

		//If item count is null when we prevent picking it at the next iteration by removing it from the list
		if(foodInfos[index].second.second == 0)
		{
			foodInfos.erase(foodInfos.begin() + index);
		}
	}


	return foodCombinations;
}

//Scans through inventory to return the accumulated foodLevel of each food items
float Trader::calculateFoodStock()
{
	float foodStock = 0.0f;
	for (const auto& item : inventory)
	{
		auto* foodItem = dynamic_cast<Food*>(item.get());
		if (foodItem != nullptr)
		{
			foodStock += foodItem->getFoodValue() * static_cast<float>(foodItem->getCount());
		}
	}
	return foodStock;
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
	int turnCount = 0;
	while (advancement < 1.0f)
	{
		advancement += craft->getRate();
		++turnCount;
	}

	return (resultPrice - requirementsPrice) / static_cast<float>(turnCount);
}

//Makes a list of items the trader wish to buy
void Trader::fillWonderList()
{
	//Food
	if (calculateFoodStock() <= 10.0f) //TODO setting
	{
		//Finds all food item that currently exists
		TradableManager* tradableManager = TradableManager::getInstance();
		std::vector < std::pair < size_t, std::pair< float, int> >> foodInfos;
		foodInfos.reserve(tradableManager->getKeys().size());

		for (auto key : tradableManager->getKeys())
		{
			auto* foodItem = dynamic_cast<Food*>(tradableManager->getTradable(key));
			if (foodItem != nullptr)
			{
				//Checks if the food item we're looking at is self-craftable or not
				bool isCraftable = false;
				auto craftableList = currentJob->getCraftableList();
				for (auto key : craftableList)
				{
					if (key == foodItem->getId())
					{
						isCraftable = true;
						const int countNeeded = std::roundf((15.0f - calculateFoodStock()) / foodItem->getFoodValue());
						auto requirements = currentJob->getCraft(foodItem->getId())->getRequirement();
						for(auto& requirement : requirements)
						{
							requirement.second *= countNeeded;
							wonderList.emplace_back(requirement);
						}
						
						break;
					}
				}
				//If not adds it to foodInfos
				if (!isCraftable)
				{
					foodInfos.emplace_back(std::pair<size_t, std::pair<float, int>>(key, std::pair<float, int>(foodItem->getFoodValue(), INT_MAX)));
				}
			}
		}

		//Send food infos to the random food combination generator and then adds to wonderList the generated combination
		const std::uniform_real_distribution<float> uniformFloatDist(1.0f, 10.0f); //TODO setting
		const float foodGoal = uniformFloatDist(randomEngine);
		wonderList.splice(wonderList.end(), getRandomFoodCombination(foodInfos, foodGoal));
	}

	//Find the most beneficial unavailable craft
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

	//If we found one, we check if requirements can be self-crafted, and if not adds it to wonderList
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

	//If trader hasn't got a tool he can use then he tries to buy it
	for (auto usableTool : this->currentJob->getUsableTools())
	{
		if (!isInInventory(usableTool))
		{
			wonderList.emplace_back(std::pair<size_t, int>(usableTool, 1));
		}
	}
}

//Makes a list of items the trader wish to sell
void Trader::fillGoodsList()
{
	for (const auto& item : inventory)
	{
		bool isRequired = false;
		//Checks if item is an usableTool
		for (auto usableTool : getCurrentJob()->getUsableTools())
		{
			if (item->getId() == usableTool)
			{
				isRequired = true;
				break;
			}
		}
		//If not checks if item is required for crafting
		if (!isRequired)
		{
			for (auto key : currentJob->getCraftList())
			{
				auto* craftable = currentJob->getCraft(key);
				for (const auto requirement : craftable->getRequirement())
				{
					if (requirement.first == item->getId())
					{
						isRequired = true;
						break;
					}
				}
			}
		}
		//If not checks if item is food
		if(!isRequired)
		{
			if(dynamic_cast<Food*>(item.get())!=nullptr)
			{
				isRequired = true;
			}
		}
		//If item is neither of that, then we can safely add it to the goodsList
		if(!isRequired)
		{
			//If it's a countable we can safely add it
			auto* countable = dynamic_cast<Countable*>(item.get());
			if (countable != nullptr)
			{
				goodsList.emplace_back(countable->getId(), countable->getCount());
			}
			//If not we check if it's id is yet present in the goodsList, and we increment its count or add it accordingly
			else
			{
				bool yetAdded = false;
				for (auto& good : goodsList)
				{
					if (good.first == item->getId())
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

	//Then we look if trader has food surplus to sell
	const float foodStock = calculateFoodStock();
	if (foodStock > 5.0f) //TODO setting
	{
		//Adds every food items possessed to foodInfos
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

		//Then send foodInfos to the random combination generator and adds the result to goodsList
		for (const auto& food : getRandomFoodCombination(foodInfos, foodStock - 5.0f)) //TODO setting
		{
			goodsList.emplace_back(std::pair<size_t, int>(food.first, food.second));
		}
	}
}

//Returns the price belief mean
float Trader::calculatePriceBeliefMean(const size_t key)
{
	auto resultPriceBelief = priceBeliefs[key];
	return (*resultPriceBelief.front() + *resultPriceBelief.back()) / 2.0f;
}

//Picks a random price in the price belief interval
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

//Makes asks
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

//Trader crafting management
void Trader::craft()
{
	if (currentJob != nullptr && currentCraft == nullptr)
	{
		const auto craftableList = this->currentJob->getCraftableList();
		if (!craftableList.empty())
		{
			//Picks randomly an available craft
			const std::uniform_int_distribution<int> uniformDist(0, static_cast<int>(craftableList.size() - 1));
			this->currentCraft = this->currentJob->createCraft(craftableList[uniformDist(randomEngine)]);

			//Init the associated tool
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
		if (currentCraft->advanceCraft())
		{
			addToInventory(currentCraft->getResult(), currentCraft->getCount());
			delete currentCraft;
			currentCraft = nullptr;
		}
	}
}

void Trader::refreshPriceBelief(Ask* ask)
{
	const float priceBeliefMean = calculatePriceBeliefMean(ask->getId());
	const float currentMean = priceHistory[ask->getId()][0]->first / static_cast<float>(priceHistory[ask->getId()][0]->second);


	if(ask->getStatus() == AskStatus::Sold)
	{
		*failCount[ask->getId()][0] = 0;
		if(ask->getPrice()*0.7 > currentMean || ask->getPrice() < currentMean * 0.7)
		{
			*priceBeliefs[ask->getId()][0] = std::max<float>(0.0f, *priceBeliefs[ask->getId()][0] + currentMean - priceBeliefMean);
			*priceBeliefs[ask->getId()][1] = std::max<float>(0.03f, *priceBeliefs[ask->getId()][1] + currentMean - priceBeliefMean);
		}
		*priceBeliefs[ask->getId()][0] = std::min<float>(currentMean-0.02f, *priceBeliefs[ask->getId()][0]+0.15f * currentMean);
		*priceBeliefs[ask->getId()][1] = std::max<float>(currentMean+0.02f, *priceBeliefs[ask->getId()][1]-0.15f * currentMean);
		priceHistory[ask->getId()][0]->first += ask->getPrice();
		++priceHistory[ask->getId()][0]->second;
	}
	else
	{
		++* failCount[ask->getId()][0];
		*priceBeliefs[ask->getId()][0] = std::max<float>(0.0f, *priceBeliefs[ask->getId()][0] + currentMean - priceBeliefMean);
		*priceBeliefs[ask->getId()][1] = std::max<float>(0.03f, *priceBeliefs[ask->getId()][1] + currentMean - priceBeliefMean);
		if(dynamic_cast<BuyingAsk*>(ask)!=nullptr)
		{	
			*priceBeliefs[ask->getId()][0] = std::min<float>(currentMean, *priceBeliefs[ask->getId()][0] + 0.05f * priceBeliefMean); //* *failCount[ask->getId()][0]);
			*priceBeliefs[ask->getId()][1] = std::min<float>(currentMean * 1.5f, *priceBeliefs[ask->getId()][1] + 0.05f * priceBeliefMean); //* *failCount[ask->getId()][0]);
		}
		else
		{
			*priceBeliefs[ask->getId()][0] = std::max<float>(currentMean*0.5f, *priceBeliefs[ask->getId()][0] - 0.05f * priceBeliefMean);// * *failCount[ask->getId()][0]);
			*priceBeliefs[ask->getId()][1] = std::max<float>(currentMean, *priceBeliefs[ask->getId()][1] - 0.05f * priceBeliefMean); //* *failCount[ask->getId()][0]);
		}
	}
}

void Trader::checkAsks()
{
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

		refreshPriceBelief(ask.get());
	}
	currentAsks.clear();
}

void Trader::refresh()
{
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

//=====Inventory Management=====
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

//=====Accessors/Mutators=====
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

float Trader::getFoodLevel() const
{
	return foodLevel;
}

float Trader::getMoney() const
{
	return money;
}
