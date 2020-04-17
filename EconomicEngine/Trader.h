#ifndef TRADER_H
#define TRADER_H

#include "Ask.h"
#include "Craft.h"
#include "Job.h"
#include "Observable.h"
#include "VectorArray.h"
#include "Tradable.h"
#include "Countable.h"
#include "Uncountable.h"

#include <list>
#include <random>

class Trader : public Observable
{
private:
	
	VectorArray<std::pair<float, int>> priceHistory;
	VectorArray<float> priceBeliefs;
	Craft* currentCraft;
	Job* currentJob;
	std::list<std::pair<size_t, int>> wonderList;
	std::list<std::pair<size_t, int>> goodsList;
	std::list<std::shared_ptr<Tradable>> inventory;
	std::list<std::shared_ptr<Ask>> currentAsks;
	mutable std::mt19937 randomEngine;
	int successCount;
	float money;
	float foodLevel;
	void fillWonderList();
	void fillGoodsList();
	void refreshPriceBelief(Ask* ask);
	
	std::list<std::pair<size_t, int>> getRandomFoodCombination(std::vector<std::pair<size_t, std::pair<float, int>>>& foodInfos, float foodGoal) const;
	float calculateEarnings(Craft* craft) const;
	float calculateFoodStock() const;
	float calculatePriceBeliefMean(size_t key) const;
	float evaluatePrice(size_t key) const;


public:
	Trader();
	Trader(Job* job);
	void makeAsks();
	void craft();
	void refresh();
	void checkAsks();
	[[nodiscard]] const std::list<std::shared_ptr<Tradable>>& getInventory() const;
	[[nodiscard]] Job* getCurrentJob() const;
	[[nodiscard]] Craft* getCurrentCraft() const;
	[[nodiscard]] bool isInInventory(size_t key);
	[[nodiscard]] float getFoodLevel() const;
	[[nodiscard]] float getMoney() const;
	int getItemCount(size_t key) const;
	void addToInventory(size_t key, int count);
	void addToInventory(Countable* countable);
	void addToInventory(Uncountable* uncountable);
	void removeFromInventory(size_t key, int count);
	void removeFromInventory(Tradable* tradable);
};

#endif

