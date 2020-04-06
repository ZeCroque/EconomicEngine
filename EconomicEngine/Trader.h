#ifndef TRADER_H
#define TRADER_H
#include <cassert>
#include <list>
#include <memory>
#include <random>


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
	std::mt19937 randomEngine;
	void assignJob();

public:
	Trader();
	void makeAsks();
	void craft();
	void refresh();
	void checkAsks();
	[[nodiscard]] const std::list<std::shared_ptr<Tradable>>& getInventory() const;
	[[nodiscard]] const Job* getCurrentJob() const;
	template <class T> void addToInventory(T* tradable)
	{	
		if (dynamic_cast<Tradable*>(tradable) == nullptr)
		{
			exit(-1);
		}
		//TODO DEBUG
		if (dynamic_cast<Countable*>(tradable) != nullptr)
		{
			for (auto& item : inventory)
			{
				auto* castedItem = dynamic_cast<T*>(item.get());

				if (castedItem != nullptr)
				{
					dynamic_cast<Countable*>(item.get())->incrementCountBy(1); //TODO multiitemcraft
					return;
				}
			}
		}

		inventory.emplace_back(tradable);
	}
	
	
};

#endif

