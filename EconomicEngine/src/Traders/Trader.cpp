#include <chrono>
#include <random>

#include "Traders/Trader.h"
#include "EconomicEngine.h"
#include "Tradables/Tradable.h"
#include "Tradables/Food.h"
#include "Tradables/Uncountable/Uncountable.h"
#include "Traders/TraderManager.h"

Trader::Trader() : bIsWaitingForActivity(true), currentAction(Action::None), position(Position::Workshop), currentJob(nullptr),successfulTradesCount(0), moneyCount(100), foodLevel(30.f)
{
	const auto& tradableFactory = EconomicEngine::getInstance()->getTradableFactory();
	auto tradablesId = tradableFactory.getKeys();
	priceBeliefs = VectorArray<float>(tradablesId);
	priceHistory = VectorArray<std::pair<float, int>>(tradablesId);
	for (auto tradableId : tradablesId)
	{
		auto tradableDefaultPriceBelief = tradableFactory.getDefaultObject(tradableId)->getDefaultPriceBelief();
		priceBeliefs[tradableId].resize(2);
		priceBeliefs[tradableId][0] = std::make_shared<float>(tradableDefaultPriceBelief.first);
		priceBeliefs[tradableId][1] = std::make_shared<float>(tradableDefaultPriceBelief.second);
		priceHistory[tradableId].emplace_back(std::make_shared<std::pair<float, int>>(getPriceBeliefMean(tradableId), 1));
	}
}

Trader::Trader(Job* inJob) : Trader()
{
	currentJob = inJob;
}

Trader::~Trader()
{
	for(auto & stockExchange = EconomicEngine::getInstance()->getStockExchange(); const auto& currentAsk : currentAsks)
	{
		stockExchange.removeAsk(currentAsk);
	}

	moveToRequestSignal.disconnectAll();
	deathSignal.disconnectAll();
}

void Trader::update(const float inDeltaTime)
{
	if(bIsWaitingForActivity)
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
		bIsWaitingForActivity = false;
	}

	// ReSharper disable once CppIncompleteSwitchStatement
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch(currentAction)  // NOLINT(clang-diagnostic-switch)
	{
		case Action::Crafting:
			if(currentCraft)
			{
				currentCraft->update(inDeltaTime);
				break;
			}

			if(!getCurrentJob()->getCraftablesIds().empty())
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
			currentAction = Action::Trading;
			[[fallthrough]];
					
		case Action::Trading:
			if(position == Position::Workshop)
			{
				position = Position::Street;
				moveToRequestSignal(Position::Market);
			}
			else if(position == Position::Market)
			{
				makeAsks();
				bIsWaitingForActivity = true;
			}
			break;
	}
}

void Trader::makeChild() const
{
	if (successfulTradesCount && successfulTradesCount % enterprisingQuietudeThreshold == 0)
	{
		EconomicEngine::getInstance()->getTraderManager().addTraders(1, currentJob->getId());
	}
}

void Trader::addToInventory(const size_t inItemId, const int inAmount)
{
	const auto& tradableFactory = EconomicEngine::getInstance()->getTradableFactory();

	auto* item = tradableFactory.createObject(inItemId);
	if (auto * countable = dynamic_cast<Countable*>(item); countable)
	{
		countable->setCount(inAmount);
		addToInventory(countable);
	}
	else
	{
		auto* uncountable = dynamic_cast<Uncountable*>(item);
		addToInventory(uncountable);
		for (auto i = 0; i < inAmount - 1; ++i)
		{
			uncountable = dynamic_cast<Uncountable*>(tradableFactory.createObject(inItemId));
			addToInventory(uncountable);
		}
	}
}

void Trader::addToInventory(Countable* inCountable)
{
	for (auto& item : inventory)
	{
		if (item->getId() == inCountable->getId())
		{
			dynamic_cast<Countable*>(item.get())->incrementCountBy(inCountable->getCount());
			return;
		}
	}
	inventory.emplace_back(inCountable);
	inCountable->setOwningTrader(this);
}

void Trader::addToInventory(Uncountable* inUncountable)
{
	inventory.emplace_back(inUncountable);
	inUncountable->setOwningTrader(this);
}

void Trader::removeFromInventory(const size_t itemId, const int inAmount = 1)
{
	for (auto& item : inventory)
	{
		if (item->getId() == itemId)
		{
			if (auto* countable = dynamic_cast<Countable*>(item.get()); countable)
			{
				countable->decrementCountBy(inAmount);
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

void Trader::removeFromInventory(Tradable* inTradable)
{
	for (auto& item : inventory)
	{
		if (item.get() == inTradable)
		{
			item.reset();
			inventory.remove(item);
			break;
		}
	}
}

bool Trader::isInInventory(const size_t inItemId)
{
	return std::ranges::any_of(inventory.begin(), inventory.end(), [inItemId](const auto& item)
	{
		return item->getId() == inItemId;
	});
}

Position Trader::getPosition() const
{
    return position;
}

Action Trader::getCurrentAction() const
{
    return currentAction;
}

Job* Trader::getCurrentJob() const
{
	return currentJob;
}

Craft* Trader::getCurrentCraft() const
{
	return currentCraft.get();
}

std::list<std::shared_ptr<Ask>> Trader::getCurrentAsks() const
{
	return currentAsks;
}

int Trader::getItemCount(const size_t inItemId) const
{
	int count = 0;
	for(const auto& item : inventory)
	{
		if(item->getId() == inItemId)
		{
			if (auto * countable = dynamic_cast<Countable*>(item.get()); countable)
			{
				count = countable->getCount();
				break;
			}
			++count;
		}
	}
	return count;
}

const std::list<std::shared_ptr<Tradable>>& Trader::getInventory() const
{
	return inventory;
}

float Trader::getFoodLevel() const
{
	return foodLevel;
}

float Trader::getMoneyCount() const
{
	return moneyCount;
}

const Signal<Position>& Trader::getMoveToRequestSignal() const
{
	return moveToRequestSignal;
}

const Signal<> &Trader::getDeathSignal() const
{
    return deathSignal;
}


void Trader::setPosition(const Position inPosition)
{
	position = inPosition;
	updateFoodLevel();
}

void Trader::updateFoodLevel()
{
	foodLevel -= foodCostPerAction;
	if (foodLevel > 0.f && foodLevel <= hungerThreshold)
	{
		std::vector<std::pair<size_t, std::pair<float, int>>> foodInfos;
		foodInfos.reserve(inventory.size());
		for (auto& item : inventory)
		{
			if (auto * foodItem = dynamic_cast<Food*>(item.get()); foodItem)
			{
				foodInfos.emplace_back(std::pair<size_t, std::pair<float, int>>(foodItem->getId(), std::pair<float, int>(foodItem->getFoodValue(), foodItem->getCount())));
			}
		}

		for (const auto& food : getRandomFoodCombination(foodInfos, hungerThreshold - foodLevel))
		{
			foodLevel += dynamic_cast<Food*>(EconomicEngine::getInstance()->getTradableFactory().getDefaultObject(food.first))->getFoodValue() * static_cast<float>(food.second);
			removeFromInventory(food.first, food.second);
		}
	}
}

void Trader::updatePriceBelief(Ask* ask)
{
	if(const float priceBeliefMean = getPriceBeliefMean(ask->getItemId()), currentMean = priceHistory[ask->getItemId()][0]->first / static_cast<float>(priceHistory[ask->getItemId()][0]->second); ask->getStatus() == AskStatus::Sold)
	{
		++successfulTradesCount;

		if(ask->getPrice() * 0.7f > currentMean || ask->getPrice() < currentMean * 0.7f)
		{
			*priceBeliefs[ask->getItemId()][0] = std::max<float>(0.0f, *priceBeliefs[ask->getItemId()][0] + currentMean - priceBeliefMean);
			*priceBeliefs[ask->getItemId()][1] = std::max<float>(0.03f, *priceBeliefs[ask->getItemId()][1] + currentMean - priceBeliefMean);
		}
		*priceBeliefs[ask->getItemId()][0] = std::min<float>(currentMean-0.02f, *priceBeliefs[ask->getItemId()][0]+0.15f * currentMean);
		*priceBeliefs[ask->getItemId()][1] = std::max<float>(currentMean+0.02f, *priceBeliefs[ask->getItemId()][1]-0.15f * currentMean);
		priceHistory[ask->getItemId()][0]->first += ask->getPrice();
		++priceHistory[ask->getItemId()][0]->second;
	}
	else
	{
		*priceBeliefs[ask->getItemId()][0] = std::max<float>(0.0f, *priceBeliefs[ask->getItemId()][0] + currentMean - priceBeliefMean);
		*priceBeliefs[ask->getItemId()][1] = std::max<float>(0.03f, *priceBeliefs[ask->getItemId()][1] + currentMean - priceBeliefMean);
		if(!ask->isSellingAsk())
		{	
			*priceBeliefs[ask->getItemId()][0] = std::min<float>(currentMean, *priceBeliefs[ask->getItemId()][0] + 0.05f * priceBeliefMean);
			*priceBeliefs[ask->getItemId()][1] = std::min<float>(currentMean * 1.5f, *priceBeliefs[ask->getItemId()][1] + 0.05f * priceBeliefMean);
		}
		else
		{
			*priceBeliefs[ask->getItemId()][0] = std::max<float>(currentMean * 0.5f, *priceBeliefs[ask->getItemId()][0] - 0.05f * priceBeliefMean);
			*priceBeliefs[ask->getItemId()][1] = std::max<float>(currentMean, *priceBeliefs[ask->getItemId()][1] - 0.05f * priceBeliefMean);
		}
	}
}

//Trader crafting management
void Trader::startCrafting()
{
	//Checks for item in surplus to avoid crafting them
	const auto craftablesIds = currentJob->getCraftablesIds();
	std::vector<size_t> interestingCrafts;
	interestingCrafts.reserve(craftablesIds.size());
	for(const auto& craftableId : craftablesIds)
	{
		if(getPriceEvaluation(craftableId) * static_cast<float>(getItemCount(craftableId)) < moneyCount * overstockFactor)
		{
			interestingCrafts.emplace_back(craftableId);
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
					if (!tool)
					{
						tool = uncountable;
						break;
					}
					auto* currentToolBehavior = dynamic_cast<ToolBehavior*>(uncountable->getBehavior());
					if (auto* toolBehavior = dynamic_cast<ToolBehavior*>(tool->getBehavior()); toolBehavior && currentToolBehavior->getCraftRateBoost() > toolBehavior->getCraftRateBoost())
					{
						tool = uncountable;
						break;
					}
				}
			}
		}
		if(tool)
		{
			auto* toolBehavior = dynamic_cast<ToolBehavior*>(tool->getBehavior());
			currentCraft->getCraftSuccessSignal().connect(toolBehavior, &ToolBehavior::updateDurability);
			currentCraft->incrementCraftingRate(toolBehavior->getCraftRateBoost());
		}

		for(const auto& requirement : currentCraft->getRequirements())
		{
			removeFromInventory(requirement.first, requirement.second);
		}
	}
}

void Trader::makeAsks()
{
	makeBuyingAsks();
	makeSellingAsks();
}

void Trader::makeBuyingAsks()
{
	std::list<std::pair<size_t, int>> wonderList;
	
	//Food
	if (getFoodStock() <= foodConcernThreshold)
	{
		//Finds all food item that currently exists
		const auto& tradableFactory = EconomicEngine::getInstance()->getTradableFactory();
		std::vector < std::pair < size_t, std::pair< float, int> >> foodInfos;
		foodInfos.reserve(tradableFactory.getKeys().size());
		bool bCanCraftFood = false;
		for (auto tradableId : tradableFactory.getKeys())
		{
			if (auto * foodItem = dynamic_cast<Food*>(tradableFactory.getDefaultObject(tradableId)); foodItem)
			{
				//Checks if the food item we're looking at is self-craftable or not
				for (const auto uncraftableId : currentJob->getUncraftablesIds())
				{
					if (uncraftableId == foodItem->getId())
					{
						bCanCraftFood = true;

						const int countNeeded = static_cast<int>(std::round((foodQuietudeThreshold - getFoodStock()) / foodItem->getFoodValue()));

						for(auto requirements = currentJob->getCraft(foodItem->getId())->getRequirements(); auto& requirement : requirements)
						{
							requirement.second *= countNeeded;
							wonderList.emplace_back(requirement);
						}

						break;
					}
				}
				if (bCanCraftFood)
				{
					break;
				}
				foodInfos.emplace_back(std::pair<size_t, std::pair<float, int>>(tradableId, std::pair<float, int>(foodItem->getFoodValue(), INT_MAX)));
			}
			
		}
		if (!bCanCraftFood)
		{
			wonderList.splice(wonderList.end(), getRandomFoodCombination(foodInfos, foodQuietudeThreshold - getFoodStock()));
		}
	}
	
	//Find the most beneficial unavailable craft
	Craft* mostBeneficialCraft = nullptr;
	for (auto uncraftableId : currentJob->getUncraftablesIds())
	{
		if (auto * craftable = currentJob->getCraft(uncraftableId); !mostBeneficialCraft || getPotentialEarnings(craftable) > getPotentialEarnings(mostBeneficialCraft))
		{
			mostBeneficialCraft = craftable;
		}
	}

	//If we found one, we check if requirements can be self-crafted, and if not adds it to wonderList
	if (mostBeneficialCraft)
	{
		for (auto requirement : mostBeneficialCraft->getRequirements())
		{
			if (!getCurrentJob()->getCraft(requirement.first))
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

	registerAsks(false, wonderList, moneyCount);
}

void Trader::makeSellingAsks()
{
	std::list<std::pair<size_t, int>> goodsList;

	for (const auto& item : inventory)
	{
		bool bIsRequired = false;
		//Checks if item is an usableTool
		for (auto usableTool : getCurrentJob()->getUsableTools())
		{
			if (item->getId() == usableTool)
			{
				bIsRequired = true;
				break;
			}
		}
		//If not checks if item is required for crafting
		if (!bIsRequired)
		{
			for (auto craftIds : currentJob->getCraftsIds())
			{
				for (auto * craftable = currentJob->getCraft(craftIds); const auto& requirement : craftable->getRequirements())
				{
					if (requirement.first == item->getId())
					{
						bIsRequired = true;
						break;
					}
				}
			}
		}
		//If not checks if item is food
		if(!bIsRequired)
		{
			if(dynamic_cast<Food*>(item.get())!=nullptr)
			{
				bIsRequired = true;
			}
		}
		//If item is neither of that, then we can safely add it to the goodsList
		if(!bIsRequired)
		{
			if (auto * countable = dynamic_cast<Countable*>(item.get()); countable)
			{
				goodsList.emplace_back(countable->getId(), countable->getCount());
			}
			//If not we check if it's id is yet present in the goodsList, and we increment its count or add it accordingly
			else
			{
				bool bYetAdded = false;
				for (auto& good : goodsList)
				{
					if (good.first == item->getId())
					{
						++good.second;
						bYetAdded = true;
						break;
					}
				}
				if (!bYetAdded)
				{
					goodsList.emplace_back(item->getId(), 1);
				}
			}
		}
	}

	if (const float foodStock = getFoodStock(); foodStock > foodQuietudeThreshold)
	{
		//Adds every food items possessed to foodInfos
		std::vector<std::pair<size_t, std::pair<float, int>>> foodInfos;
		foodInfos.reserve(inventory.size());
		for (auto& item : inventory)
		{
			if (auto * foodItem = dynamic_cast<Food*>(item.get()); foodItem)
			{
				foodInfos.emplace_back(std::pair<size_t, std::pair<float, int>>(foodItem->getId(), std::pair<float, int>(foodItem->getFoodValue(), foodItem->getCount())));
			}
		}

		//Then send foodInfos to the random combination generator and adds the result to goodsList
		for (const auto& food : getRandomFoodCombination(foodInfos, foodStock - (foodQuietudeThreshold - foodConcernThreshold) - 1))
		{
			goodsList.emplace_back(std::pair<size_t, int>(food.first, food.second));
		}
	}

	registerAsks(true, goodsList, std::numeric_limits<float>::max());
}

void Trader::registerAsks(bool bInIsSellingAsk, const std::list<std::pair<size_t, int>>& inItems,
	const float inMaxPrice)
{
	for (const auto& item : inItems)
	{
		if (float price = getPriceEvaluation(item.first); price <= inMaxPrice)
		{
			auto ask = std::make_shared<Ask>(bInIsSellingAsk, item.first, item.second, price);
			currentAsks.push_back(ask);
			EconomicEngine::getInstance()->getStockExchange().registerAsk(ask);
			ask->getAskResolvedSignal().connect(this, &Trader::checkAskCallback);		
		}
	}
}

void Trader::checkAskCallback(Ask* inAsk)
{
	if (inAsk->getStatus() == AskStatus::Sold)
	{
		if (!inAsk->isSellingAsk())
		{
			addToInventory(inAsk->getItemId(), inAsk->getCount());
			moneyCount -= inAsk->getPrice() * static_cast<float>(inAsk->getCount());
		}
		else
		{
			removeFromInventory(inAsk->getItemId(), inAsk->getTradedCount());
			moneyCount += inAsk->getPrice() * static_cast<float>(inAsk->getTradedCount());
		}
	}
	updatePriceBelief(inAsk);

	currentAsks.remove_if([inAsk](const std::shared_ptr<Ask>& value)
	{
		return value.get() == inAsk;
	});
}

void Trader::craftSuccessCallback()
{
	addToInventory(currentCraft->getResultId(), currentCraft->getCount());
	currentCraft.release();  // NOLINT(bugprone-unused-return-value)
	updateFoodLevel();
	bIsWaitingForActivity = true;
}

std::list<std::pair<size_t, int>> Trader::getRandomFoodCombination(std::vector<std::pair<size_t, std::pair<float, int>>>& inFoodInfos, const float inFoodGoal)
{
	std::mt19937 randomEngine(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	std::list<std::pair<size_t, int>> foodCombinations;

	float foodCount = 0.0f;
	while (foodCount < inFoodGoal && !inFoodInfos.empty())
	{
		//Picks a food randomly in foodInfos
		std::uniform_int_distribution<int> uniformIntDist(0, static_cast<int>(inFoodInfos.size() - 1));
		const int index = uniformIntDist(randomEngine);

		//Establish the food count, that is random between 1 and either the number of items necessary to fulfill the food goal or the max count of food provided in entry table
		uniformIntDist = std::uniform_int_distribution<int>(1, std::max<int>(1, std::min<int>(inFoodInfos[index].second.second, static_cast<int>(std::round(inFoodGoal / inFoodInfos[index].second.first)))));
		const int count = uniformIntDist(randomEngine);

		//Decrease food count of picked item
		inFoodInfos[index].second.second-=count;

		//Update the foodCount from the provided data
		foodCount += inFoodInfos[index].second.first * static_cast<float>(count);

		//Then checks if the item we picked were previously picked or not, increment it or adds it to combination accordingly
		bool bYetAdded = false;
		for(auto& foodCombination : foodCombinations)
		{
			if(foodCombination.first == inFoodInfos[index].first)
			{
				bYetAdded = true;
				++foodCombination.second;
			}
		}
		if(!bYetAdded)
		{
			foodCombinations.emplace_back(inFoodInfos[index].first, count);
		}

		//If item count is null when we prevent picking it at the next iteration by removing it from the list
		if(inFoodInfos[index].second.second == 0)
		{
			inFoodInfos.erase(inFoodInfos.begin() + index);
		}
	}


	return foodCombinations;
}

float Trader::getFoodStock() const
{
	float foodStock = 0.0f;
	for (const auto& item : inventory)
	{
		if (auto * foodItem = dynamic_cast<Food*>(item.get()); foodItem)
		{
			foodStock += foodItem->getFoodValue() * static_cast<float>(foodItem->getCount());
		}
	}
	return foodStock;
}

float Trader::getPotentialEarnings(Craft* inCraft) const
{
	const float resultPrice = getPriceBeliefMean(inCraft->getResultId());

	float requirementsPrice = 0;
	for (const auto& requirement : inCraft->getRequirements())
	{
		requirementsPrice += getPriceBeliefMean(requirement.first) * static_cast<float>(requirement.second);
	}

	float advancement = 0.0f;
	int updatesCount = 0;
	while (advancement < 1.0f)
	{
		advancement += inCraft->getCraftingRate();
		++updatesCount;
	}

	return (resultPrice - requirementsPrice) / static_cast<float>(updatesCount);
}

float Trader::getPriceBeliefMean(const size_t inTradableId) const
{
	auto resultPriceBelief = priceBeliefs[inTradableId];
	return (*resultPriceBelief.front() + *resultPriceBelief.back()) / 2.0f;
}

float Trader::getPriceEvaluation(const size_t inTradableId) const
{
	std::mt19937 randomEngine(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	const std::uniform_int_distribution<int> uniformIntDist(0, 1);
	std::uniform_real_distribution<float> uniformFloatDist;
	if (const float mean = getPriceBeliefMean(inTradableId); uniformIntDist(randomEngine))
	{
		uniformFloatDist = std::uniform_real_distribution<float>(*priceBeliefs[inTradableId].front(), mean);
	}
	else
	{
		uniformFloatDist = std::uniform_real_distribution<float>(mean, *priceBeliefs[inTradableId].back());
	}
	return uniformFloatDist(randomEngine);
}