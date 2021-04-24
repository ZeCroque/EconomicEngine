#include <chrono>
#include <random>
#include "Traders/Trader.h"




#include "EconomicEngine.h"
#include "Tradables/Tradable.h"
#include "Tradables/Food.h"
#include "Tradables/Uncountable/Uncountable.h"
#include "Traders/TraderManager.h"

Trader::Trader() : isWaitingForActivity(true), currentAction(Action::None), currentJob(nullptr), successCount(0),money(100), foodLevel(30.f), position(Position::Workshop)
{
	const auto& tradableManager = EconomicEngine::getInstance()->getTradableFactory();
	auto keys = tradableManager.getKeys();
	priceBeliefs = VectorArray<float>(keys);
	priceHistory = VectorArray<std::pair<float, int>>(keys);
	for (auto key : keys)
	{
		auto tradableDefaultPriceBelief = tradableManager.getDefaultObject(key)->getDefaultPriceBelief();
		priceBeliefs[key].resize(2);
		priceBeliefs[key][0] = std::make_shared<float>(tradableDefaultPriceBelief.first);
		priceBeliefs[key][1] = std::make_shared<float>(tradableDefaultPriceBelief.second);
		priceHistory[key].emplace_back(std::make_shared<std::pair<float, int>>(calculatePriceBeliefMean(key), 1));
	}
}

Trader::Trader(Job* job) : Trader()
{
	currentJob = job;
}

Trader::~Trader()
{
	auto& stockExchange = EconomicEngine::getInstance()->getStockExchange();
	for(const auto& currentAsk : currentAsks)
	{
		stockExchange.removeAsk(currentAsk);
	}
	++EconomicEngine::getInstance()->getTraderManager().demographyCounts[getCurrentJob()->getId()][0]->second;

	while(position == Position::Street);
	moveToRequestSignal.disconnectAll();
	//TODO notify death
}

void Trader::update(const float deltaTime)
{
	if(isWaitingForActivity)
	{
		switch(currentAction)
		{
			case Action::None:
				currentAction = Action::Crafting;
				break;
			case Action::Crafting:
				currentAction = Action::Trading;
				break;
			case Action::Trading:
				currentAction = Action::Crafting;
				break;
			case Action::Sleeping:
				break;
		}
		isWaitingForActivity = false;
	}

	// ReSharper disable once CppIncompleteSwitchStatement
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch(currentAction)  // NOLINT(clang-diagnostic-switch) //TODO better action decision (use goodsList to prefer trading over crafting when above threshold
	{
		case Action::Crafting:
			if(!currentCraft)
			{
				if(!getCurrentJob()->getCraftableList().empty())
				{				
					if(position == Position::Market)
					{
						position = Position::Street;
						moveToRequestSignal(Position::Workshop);
					}
					else if(position == Position::Workshop)
					{
						startCrafting();
					}
					break;				
				}		
				if(!getCurrentAsks().empty())
				{
					break;
				}
				//If no craft is available and no trade awaiting then trade
				[[fallthrough]];
			}
			else
			{
				currentCraft->update(deltaTime);
				break;
			}		
		case Action::Trading:
			if(position == Position::Workshop)
			{
				position = Position::Street;
				moveToRequestSignal(Position::Market);
			}
			else if(position == Position::Market)
			{
				makeAsks();
				isWaitingForActivity = true;
			}
			break;
	}
}

//Return a random combination of food to satisfy the provided foodGoal, according to the limits specified in provided foodInfos
std::list<std::pair<size_t, int>> Trader::getRandomFoodCombination(std::vector<std::pair<size_t, std::pair<float, int>>>& foodInfos, const float foodGoal)
{
	std::mt19937 randomEngine(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	std::list<std::pair<size_t, int>> foodCombinations;

	float foodCount = 0.0f;
	while (foodCount < foodGoal && !foodInfos.empty())
	{
		//Picks a food randomly in foodInfos
		std::uniform_int_distribution<int> uniformIntDist(0, static_cast<int>(foodInfos.size() - 1));
		const int index = uniformIntDist(randomEngine);

		//Establish the food count, that is random between 1 and either the number of items necessary to fulfill the food goal or the max count of food provided in entry table
		uniformIntDist = std::uniform_int_distribution<int>(1, std::max<int>(1, std::min<int>(foodInfos[index].second.second, static_cast<int>(std::round(foodGoal / foodInfos[index].second.first)))));
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
float Trader::calculateFoodStock() const
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

float Trader::calculateEarnings(Craft* craft) const
{
	const float resultPrice = calculatePriceBeliefMean(craft->getResult());

	float requirementsPrice = 0;
	for (const auto& requirement : craft->getRequirement())
	{
		requirementsPrice += calculatePriceBeliefMean(requirement.first) * static_cast<float>(requirement.second);
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
void Trader::makeBuyingAsks()
{
	std::list<std::pair<size_t, int>> wonderList;
	
	//Food
	if (calculateFoodStock() <= 10.0f) //TODO setting
	{
		//Finds all food item that currently exists
		const auto& tradableManager = EconomicEngine::getInstance()->getTradableFactory();
		std::vector < std::pair < size_t, std::pair< float, int> >> foodInfos;
		foodInfos.reserve(tradableManager.getKeys().size());
		bool canCraftFood = false;
		for (auto key : tradableManager.getKeys())
		{
			auto* foodItem = dynamic_cast<Food*>(tradableManager.getDefaultObject(key));
			if (foodItem != nullptr)
			{
				//Checks if the food item we're looking at is self-craftable or not

				auto uncraftableList = currentJob->getUncraftableList();
				for (const auto result : uncraftableList)
				{
					if (result== foodItem->getId())
					{
						canCraftFood = true;

						const int countNeeded = static_cast<int>(std::round((15.0f - calculateFoodStock()) / foodItem->getFoodValue()));

						auto requirements = currentJob->getCraft(foodItem->getId())->getRequirement();
						for(auto& requirement : requirements)
						{
							requirement.second *= countNeeded;
							wonderList.emplace_back(requirement);
						}

						break;
					}
				}
				if (canCraftFood)
				{
					break;
				}
				foodInfos.emplace_back(std::pair<size_t, std::pair<float, int>>(key, std::pair<float, int>(foodItem->getFoodValue(), INT_MAX)));
			}
			
		}
		if (!canCraftFood)
		{
			wonderList.splice(wonderList.end(), getRandomFoodCombination(foodInfos, 15.0f - calculateFoodStock()));
		}
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
			if (getCurrentJob()->getCraft(requirement.first) == nullptr)
			{
				wonderList.emplace_back(requirement);
			}
		}
	}

	//If trader hasn't got a tool he can use then he tries to buy it
	for (auto usableTool : currentJob->getUsableTools())
	{
		if (!isInInventory(usableTool))
		{
			wonderList.emplace_back(std::pair<size_t, int>(usableTool, 1));
		}
	}

	registerAsks(false, wonderList, money);
}

//Makes a list of items the trader wish to sell
void Trader::makeSellingAsks()
{
	std::list<std::pair<size_t, int>> goodsList;

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
				for (const auto& requirement : craftable->getRequirement())
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

	registerAsks(true, goodsList, std::numeric_limits<float>::max());
}

//Returns the price belief mean
float Trader::calculatePriceBeliefMean(const size_t key) const
{
	auto resultPriceBelief = priceBeliefs[key];
	return (*resultPriceBelief.front() + *resultPriceBelief.back()) / 2.0f;
}

//Picks a random price in the price belief interval
float Trader::evaluatePrice(const size_t key) const
{
	std::mt19937 randomEngine(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
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
	makeBuyingAsks();
	makeSellingAsks();
}

//Trader crafting management
void Trader::startCrafting()
{
	const auto craftableList = currentJob->getCraftableList();
	std::vector<size_t> interestingCrafts;
	interestingCrafts.reserve(craftableList.size());
	for(const auto& key : craftableList)
	{
		if(evaluatePrice(key) * static_cast<float>(getItemCount(key)) < money * 2.0f) //TODO settings
		{
			interestingCrafts.emplace_back(key);
		}
	}

	if (!interestingCrafts.empty())
	{
		std::mt19937 randomEngine(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		//Picks randomly an available craft
		const std::uniform_int_distribution<int> uniformDist(0, static_cast<int>(interestingCrafts.size() - 1));
		currentCraft = std::unique_ptr<Craft>(currentJob->createCraft(interestingCrafts[uniformDist(randomEngine)]));
		currentCraft->getCraftSuccessSignal().connect(this, &Trader::craftSuccessCallback);
		
		//Init the associated tool
		Uncountable* tool = nullptr;
		for (auto& item : inventory)
		{
			for (auto usableTool : currentJob->getUsableTools())
			{
				if (usableTool == item->getId())
				{
					auto* uncountable = dynamic_cast<Uncountable*>(item.get());
					if (tool == nullptr)
					{
						tool = uncountable;
						break;
					}
					auto* currentToolBehavior = dynamic_cast<ToolBehavior*>(uncountable->getBehavior());
					auto* toolBehavior = dynamic_cast<ToolBehavior*>(tool->getBehavior());
					if (currentToolBehavior->getCraftRateBoost() > toolBehavior->getCraftRateBoost())
					{
						tool = uncountable;
						break;
					}
				}
			}
		}
		if(tool!=nullptr)
		{
			auto* toolBehavior = dynamic_cast<ToolBehavior*>(tool->getBehavior());
			currentCraft->getCraftSuccessSignal().connect(toolBehavior, &ToolBehavior::updateToolDurability);
			currentCraft->incrementRate(toolBehavior->getCraftRateBoost());
		}

		for(const auto& requirement : currentCraft->getRequirement())
		{
			removeFromInventory(requirement.first, requirement.second);
		}
	}
}

void Trader::updatePriceBelief(Ask* ask)
{
	const float priceBeliefMean = calculatePriceBeliefMean(ask->getId());
	const float currentMean = priceHistory[ask->getId()][0]->first / static_cast<float>(priceHistory[ask->getId()][0]->second);


	if(ask->getStatus() == AskStatus::Sold)
	{
		++successCount;

		if(ask->getPrice() * 0.7f > currentMean || ask->getPrice() < currentMean * 0.7f)
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
		*priceBeliefs[ask->getId()][0] = std::max<float>(0.0f, *priceBeliefs[ask->getId()][0] + currentMean - priceBeliefMean);
		*priceBeliefs[ask->getId()][1] = std::max<float>(0.03f, *priceBeliefs[ask->getId()][1] + currentMean - priceBeliefMean);
		if(!ask->getIsSellingAsk())
		{	
			*priceBeliefs[ask->getId()][0] = std::min<float>(currentMean, *priceBeliefs[ask->getId()][0] + 0.05f * priceBeliefMean);
			*priceBeliefs[ask->getId()][1] = std::min<float>(currentMean * 1.5f, *priceBeliefs[ask->getId()][1] + 0.05f * priceBeliefMean);
		}
		else
		{
			*priceBeliefs[ask->getId()][0] = std::max<float>(currentMean * 0.5f, *priceBeliefs[ask->getId()][0] - 0.05f * priceBeliefMean);
			*priceBeliefs[ask->getId()][1] = std::max<float>(currentMean, *priceBeliefs[ask->getId()][1] - 0.05f * priceBeliefMean);
		}
	}
}

void Trader::updateFoodLevel()
{
	foodLevel -= 0.34f;
	if (foodLevel > 0.f && foodLevel <= 10.f)
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

		for (const auto& food : getRandomFoodCombination(foodInfos, 10.0f - foodLevel))
		{
			foodLevel += dynamic_cast<Food*>(EconomicEngine::getInstance()->getTradableFactory().getDefaultObject(food.first))->getFoodValue() * static_cast<float>(food.second);
			removeFromInventory(food.first, food.second);
		}
	}
}

void Trader::makeChild()
{
	if (successCount > 10)
	{
		successCount = 0;
		EconomicEngine::getInstance()->getTraderManager().addTrader(1, currentJob->getId());
	}
}

void Trader::checkAskCallback(Ask* ask)
{
	if (ask->getStatus() == AskStatus::Sold)
	{
		if (!ask->getIsSellingAsk())
		{
			addToInventory(ask->getId(), ask->getCount());
			money -= ask->getPrice() * static_cast<float>(ask->getCount());
		}
		else
		{
			removeFromInventory(ask->getId(), ask->getTradedCount());
			money += ask->getPrice() * static_cast<float>(ask->getTradedCount());
		}
	}
	updatePriceBelief(ask);

	currentAsks.remove_if([ask](const std::shared_ptr<Ask>& value)
	{
		return value.get() == ask;
	});
}

void Trader::craftSuccessCallback()
{
	addToInventory(currentCraft->getResult(), currentCraft->getCount());
	currentCraft.release();  // NOLINT(bugprone-unused-return-value)
	updateFoodLevel();
	isWaitingForActivity = true;
}

void Trader::setPosition(const Position inPosition)
{
	position = inPosition;
	updateFoodLevel();
}

int  Trader::getItemCount(const size_t key) const
{
	int count = 0;
	for(const auto& item : inventory)
	{
		if(item->getId() == key)
		{
			auto* countable = dynamic_cast<Countable*>(item.get());
			if (countable != nullptr)
			{
				count = countable->getCount();
				break;
			}
			++count;
		}
	}
	return count;
}

const Signal<Position>& Trader::getMoveToRequestSignal() const
{
	return moveToRequestSignal;
}

std::list<std::shared_ptr<Ask>> Trader::getCurrentAsks() const
{
	return currentAsks;
}

bool Trader::isInInventory(const size_t key)
{
	for(const auto& item : inventory)
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
	const auto& tradableManager = EconomicEngine::getInstance()->getTradableFactory();

	auto* item = tradableManager.createObject(key);
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
			uncountable = dynamic_cast<Uncountable*>(tradableManager.createObject(key));
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
	countable->setOwningTrader(this);
}

void Trader::addToInventory(Uncountable* uncountable)
{
	inventory.emplace_back(uncountable);
	uncountable->setOwningTrader(this);
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

const std::list<std::shared_ptr<Tradable>>& Trader::getInventory() const
{
	return inventory;
}

Job* Trader::getCurrentJob() const
{
	return currentJob;
}

Craft* Trader::getCurrentCraft() const
{
	return currentCraft.get();
}

float Trader::getFoodLevel() const
{
	return foodLevel;
}

float Trader::getMoney() const
{
	return money;
}
