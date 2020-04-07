#ifndef TRADER_H
#define TRADER_H
#include <cassert>
#include <list>
#include <memory>
#include <random>


#include "Ask.h"
#include "Craft.h"
#include "Job.h"
#include "VectorArray.h"

class Job;

class Trader
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
	void assignJob();
	void fillWonderList();
	float calculateEarnings(Craft* craft);

public:
	Trader();
	void makeAsks();
	void craft();
	void refresh();
	void checkAsks();
	[[nodiscard]] const std::list<std::shared_ptr<Tradable>>& getInventory() const;
	[[nodiscard]] const Job* getCurrentJob() const;
	void addToInventory(Tradable* tradable);
	void removeFromInventory(size_t key);
};

#endif

