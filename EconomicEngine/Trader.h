#ifndef TRADER_H
#define TRADER_H
#include <list>
#include <map>

#include "AskFactory.h"
#include "Craft.h"
#include "Job.h"
#include "Money.h"


class Trader
{
private:
	std::pair<int, int> priceBelief;
	Craft* currentCraft;
	Job* currentJob;
	//Ask* currentAsks;
	//std::list<std::reference_wrapper<AskFactory>> askFactories;
	std::list<std::reference_wrapper<Tradable>> inventory;

public:
	void makeAsks();
	void craft();
	void refresh();
	void checkAsks();
	
	
};

#endif

