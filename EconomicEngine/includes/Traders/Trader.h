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
	Sleeping = 3 //TODO trader sleeping
};

enum class Position
{
	Workshop = 0,
	Market = 1,
	Street = 2
};

class Trader
{
public:
	Trader();
	
	explicit Trader(Job* inJob);
	
	Trader(Trader&&) = default;
	
	~Trader();

	void update(float inDeltaTime);
	
	void makeChild() const;

	void addToInventory(size_t inItemId, int inAmount);
	
	void addToInventory(class Countable* inCountable);
	
	void addToInventory(class Uncountable* inUncountable);
	
	void removeFromInventory(size_t itemId, int inAmount);
	
	void removeFromInventory(Tradable* inTradable);

	[[nodiscard]] bool isInInventory(size_t inItemId);
	
	Position getPosition() const;
	
    Action getCurrentAction() const;
	
	[[nodiscard]] Job* getCurrentJob() const;
	
	[[nodiscard]] Craft* getCurrentCraft() const;

	[[nodiscard]] std::list<std::shared_ptr<Ask>> getCurrentAsks() const;

	[[nodiscard]] const std::list<std::shared_ptr<Tradable>>& getInventory() const;

	[[nodiscard]] int getItemCount(size_t inItemId) const;
	
	[[nodiscard]] float getFoodLevel() const;
	
	[[nodiscard]] float getMoneyCount() const;

	[[nodiscard]] const Signal<Position>& getMoveToRequestSignal() const;
	
    [[nodiscard]] const Signal<>& getDeathSignal() const;

	void setPosition(Position inPosition);
	
private:

	void updateFoodLevel();
	
	void updatePriceBelief(Ask* ask);
	
	void startCrafting();

	void makeAsks();

	void makeBuyingAsks();
	
	void makeSellingAsks();

	void registerAsks(bool bInIsSellingAsk, const std::list<std::pair<size_t, int>>& inItems, const float inMaxPrice);

	void checkAskCallback(Ask* inAsk);
	
	void craftSuccessCallback();

	//Return a random combination of food to satisfy the provided foodGoal, according to the limits specified in provided foodInfos
	static std::list<std::pair<size_t, int>> getRandomFoodCombination(std::vector<std::pair<size_t, std::pair<float, int>>>& inFoodInfos, float inFoodGoal);
	
	//Scans through inventory to return the accumulated foodLevel of each food items
	[[nodiscard]] float getFoodStock() const;

	[[nodiscard]] float getPotentialEarnings(Craft* inCraft) const;
	
	[[nodiscard]] float getPriceBeliefMean(size_t inTradableId) const;
	
	[[nodiscard]] float getPriceEvaluation(size_t inTradableId) const;
	
	bool bIsWaitingForActivity;
	Action currentAction;
	Position position;
	std::unique_ptr<Craft> currentCraft;
	Job* currentJob;
	std::list<std::shared_ptr<Ask>> currentAsks;
	
	VectorArray<std::pair<float, int>> priceHistory;
	VectorArray<float> priceBeliefs;
	int successfulTradesCount;

	std::list<std::shared_ptr<Tradable>> inventory;
	float moneyCount;
	float foodLevel;

	Signal<Position> moveToRequestSignal;
    Signal<> deathSignal;

	//Threshold under which traders will be worried about starvation
	inline static float foodConcernThreshold = 10.f;

	//Threshold above which traders will consider themselves safe from hunger
	inline static float foodQuietudeThreshold = 15.f;

	//Threshold under which traders will be hungry
	inline static float hungerThreshold = 10.f;
	
	inline static float foodCostPerAction = 0.34f;

	//Threshold above which traders will consider their working situation as stable
	inline static int enterprisingQuietudeThreshold = 10;

	//Factor used to determine if a tradable is overstocked relatively to the trader money amount
	inline static float overstockFactor = 2.f;
};

#endif //TRADER_H

