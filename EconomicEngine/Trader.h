#ifndef TRADER_H
#define TRADER_H
#include <list>
#include <map>
#include <memory>

#include "Ask.h"
#include "Craft.h"
#include "Job.h"

class Job;

class Trader
{
private:
	std::pair<int, int> priceBelief;
	Craft* currentCraft;
	Job* currentJob;
	std::list<size_t> wonderList;
	std::list<size_t> goodsList;
	std::list<std::shared_ptr<Tradable>> inventory;
	std::list<std::shared_ptr<Ask>> currentAsks;
	size_t a; //DEBUG
	void assignJob();

public:
	Trader();
	void makeAsks();
	void craft();
	void refresh();
	void checkAsks();
	[[nodiscard]] const std::list<std::shared_ptr<Tradable>>& getInventory() const;
	[[nodiscard]] const Job* getCurrentJob() const;
	
	
};

#endif

