#ifndef TRADER_H
#define TRADER_H

#include <list>

#include "Craft.h"
#include "Job.h"
#include "VectorArray.h"
#include "EconomicEngine.h"
#include "StockExchange/StockExchange.h"

enum class Action
{
	None = 0,
	Crafting = 1,
	Trading = 2,
	Sleeping = 3
};

enum class Position
{
	Workshop = 0,
	Market = 1,
	Street = 2
};

class Trader
{
private:

	bool bIsWaitingForActivity;
	Action currentAction;
	
	VectorArray<std::pair<float, int>> priceHistory;
	VectorArray<float> priceBeliefs;
	std::unique_ptr<Craft> currentCraft;
	Job* currentJob;
	std::list<std::shared_ptr<Ask>> currentAsks;
	std::list<std::shared_ptr<Tradable>> inventory;
	int successCount;
	float money;
	float foodLevel;
	Position position;
	
	void makeBuyingAsks();
	void makeSellingAsks();
	void updatePriceBelief(Ask* ask);

	static std::list<std::pair<size_t, int>> getRandomFoodCombination(std::vector<std::pair<size_t, std::pair<float, int>>>& inFoodInfos, float inFoodGoal) ;
	[[nodiscard]] float calculateEarnings(Craft* inCraft) const;
	[[nodiscard]] float calculateFoodStock() const;
	[[nodiscard]] float calculatePriceBeliefMean(size_t inKey) const;
	[[nodiscard]] float evaluatePrice(size_t key) const;
	Signal<Position> moveToRequestSignal;
    Signal<> deathSignal;

    void registerAsks(bool bInIsSellingAsk, const std::list<std::pair<size_t, int>>& itemList, const float maxPrice)
	{
		for (const auto& item : itemList)
		{
			if (float price = evaluatePrice(item.first); price <= maxPrice)
			{
				auto ask = std::make_shared<Ask>(bInIsSellingAsk, item.first, item.second, price);
				currentAsks.push_back(ask);
				EconomicEngine::getInstance()->getStockExchange().registerAsk(ask);
				ask->getAskResolvedSignal().connect(this, &Trader::checkAskCallback);		
			}
		}
	}
	
public:
	Trader();
	explicit Trader(Job* inJob);
	Trader(Trader&&) = default;
	~Trader();

	void update(float inDeltaTime);
	void makeAsks();
	void makeChild();
	void startCrafting();
	void updateFoodLevel();
	void checkAskCallback(Ask* inAsk);
	void craftSuccessCallback();
	void setPosition(Position inPosition);
	Position getPosition() const;
    Action getCurrentAction() const;
	[[nodiscard]] const std::list<std::shared_ptr<Tradable>>& getInventory() const;
	[[nodiscard]] Job* getCurrentJob() const;
	[[nodiscard]] Craft* getCurrentCraft() const;
	[[nodiscard]] bool isInInventory(size_t inKey);
	[[nodiscard]] float getFoodLevel() const;
	[[nodiscard]] float getMoney() const;
	[[nodiscard]] int getItemCount(size_t inKey) const;
	[[nodiscard]] const Signal<Position>& getMoveToRequestSignal() const;
    [[nodiscard]] const Signal<>& getDeathSignal() const;
	[[nodiscard]] std::list<std::shared_ptr<Ask>> getCurrentAsks() const;
	void addToInventory(size_t inKey, int inCount);
	void addToInventory(class Countable* inCountable);
	void addToInventory(class Uncountable* inUncountable);
	void removeFromInventory(size_t inKey, int inCount);
	void removeFromInventory(Tradable* inTradable);
};

#endif

