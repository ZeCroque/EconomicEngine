#ifndef TRADER_H
#define TRADER_H
#include <list>
#include <map>
#include "Ask.h"
#include "Craft.h"
#include "Job.h"


class Trader
{
private:
	std::pair<int, int> priceBelief;
	Craft* currentCraft;
	Job* currentJob;
	std::list<std::reference_wrapper<Tradable>> wonderList;
	std::list<std::reference_wrapper<Tradable>> goodsList;
	std::list<std::reference_wrapper<Tradable>> inventory;
	std::list<std::reference_wrapper<Ask>> currentAsks;

public:
	Trader();
	void makeAsks();
	void craft();
	void refresh();
	void checkAsks();
	
	
};

#endif

