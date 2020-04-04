#include "TurnManager.h"

#include <memory>
#include <thread>

#include "Trader.h"
#include "Countable.h"
//#include "CraftFactory.h"
#include "Money.h"
#include "Uncountable.h"

void TurnManager::init()
{

	std::vector<std::pair<size_t, int>> requirements;
	requirements.emplace_back(std::pair<size_t, int>(typeid(Countable).hash_code(), 1));
	requirements.emplace_back(std::pair<size_t, int>(typeid(Money).hash_code(), 2));
	Craft craft(0.34f, typeid(Uncountable).hash_code(), requirements);

	Trader a;
	/*a.inventory.emplace_back(std::make_shared<Countable>(1));
	a.inventory.emplace_back(std::make_shared<Money>(2));
	
	CraftFactory fact(a);
	fact.registerObject(typeid(Uncountable).hash_code(), &craft);
	
	bool result = fact.isCraftable(typeid(Uncountable).hash_code());
	
	auto* item = dynamic_cast<Countable*>(a.inventory.back().get());
	item->count = 1;
	result = fact.isCraftable(typeid(Uncountable).hash_code());

	getchar();*/
	
}

void TurnManager::reset()
{
	//TODO
}


//TODO turncount defined by economic engine
int TurnManager::exec()
{
	this->bRunning = true;
	while(bRunning)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		this->notifyObservers();		
	}
	return 0;
}

void TurnManager::stop()
{
	this->bRunning = false;
}

