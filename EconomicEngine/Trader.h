#ifndef TRADER_H
#define TRADER_H

#include <list>
#include <random>


#include "Ask.h"
#include "Craft.h"
#include "Job.h"
#include "Observable.h"
#include "VectorArray.h"

class Countable;
class Uncountable;
class Job;

class Trader : public Observable
{
private:
	VectorArray<float> priceBeliefs;
	Craft* currentCraft;
	Job* currentJob;
	std::list<std::pair<size_t, int>> wonderList;
	std::list<std::pair<size_t, int>> goodsList;
	std::list<std::shared_ptr<Tradable>> inventory;
	std::list<std::shared_ptr<Ask>> currentAsks;
	std::mt19937 randomEngine;
	float money;
	float foodLevel;
	void assignJob();
	void fillWonderList();
	void fillGoodsList();
	std::list<std::pair<size_t, int>> getRandomFoodCombination(const std::vector<std::pair<size_t, std::pair<float,int>>>& foodInfos, float foodGoal);
	float calculateFoodStock();
	float calculatePriceBeliefMean(size_t key);
	float evaluatePrice(size_t key);
	float calculateEarnings(Craft* craft);

public:
	Trader();
	void makeAsks();
	void craft();
	void refresh();
	void checkAsks();
	[[nodiscard]] const std::list<std::shared_ptr<Tradable>>& getInventory() const;
	[[nodiscard]] Job* getCurrentJob() const;
	[[nodiscard]] Craft* getCurrentCraft() const;
	[[nodiscard]] bool isInInventory(size_t key);
	[[nodiscard]] float getFoodLevel() const;
	void addToInventory(size_t key, int count);
	void addToInventory(Countable* countable);
	void addToInventory(Uncountable* uncountable);
	void removeFromInventory(size_t key, int count);
	void removeFromInventory(Tradable* tradable);
};

#endif

