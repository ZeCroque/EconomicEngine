#ifndef TRADER_H
#define TRADER_H

#include "Craft.h"
#include "Job.h"
#include "VectorArray.h"

#include <list>


#include "EconomicEngine.h"
#include "StockExchange/StockExchange.h"

enum class Action
{
	None = 0,
	Crafting = 1,
	Trading = 2,
	Sleeping = 3
};

class Trader
{
private:

	bool isWaitingForActivity;
	Action currentAction;
	
	VectorArray<std::pair<float, int>> priceHistory;
	VectorArray<float> priceBeliefs;
	std::unique_ptr<Craft> currentCraft;
	Job* currentJob;
	std::list<std::shared_ptr<Tradable>> inventory;
	int successCount;
	float money;
	float foodLevel;
	void makeBuyingAsks();
	void makeSellingAsks();
	void updatePriceBelief(Ask* ask);
	void makeChild();
	static std::list<std::pair<size_t, int>> getRandomFoodCombination(std::vector<std::pair<size_t, std::pair<float, int>>>& foodInfos, float foodGoal) ;
	[[nodiscard]] float calculateEarnings(Craft* craft) const;
	[[nodiscard]] float calculateFoodStock() const;
	[[nodiscard]] float calculatePriceBeliefMean(size_t key) const;
	[[nodiscard]] float evaluatePrice(size_t key) const;

	template<class T> void registerAsks(const std::list<std::pair<size_t, int>>& itemList, const float maxPrice)
	{
		for (const auto& item : itemList)
		{
			float price = evaluatePrice(item.first);
			if (price <= maxPrice)
			{
				auto ask = std::make_shared<T>(item.first, item.second, price);
				EconomicEngine::getInstance()->getStockExchange().registerAsk(ask);
				ask->getAskResolvedSignal().connect(this, &Trader::checkAskCallback);
			}
		}
	}
	
public:
	Trader();
	explicit Trader(Job* job);

	void update(float deltaTime);
	void makeAsks();
	void startCrafting();
	void updateFoodLevel();
	void checkAskCallback(Ask* ask);
	void craftSuccessCallback();
	[[nodiscard]] const std::list<std::shared_ptr<Tradable>>& getInventory() const;
	[[nodiscard]] Job* getCurrentJob() const;
	[[nodiscard]] Craft* getCurrentCraft() const;
	[[nodiscard]] bool isInInventory(size_t key);
	[[nodiscard]] float getFoodLevel() const;
	[[nodiscard]] float getMoney() const;
	[[nodiscard]] int getItemCount(size_t key) const;
	void addToInventory(size_t key, int count);
	void addToInventory(class Countable* countable);
	void addToInventory(class Uncountable* uncountable);
	void removeFromInventory(size_t key, int count);
	void removeFromInventory(Tradable* tradable);
};

#endif

