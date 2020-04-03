#include "TurnManager.h"
#include <thread>

#include "Trader.h"
#include "Ask.h"
#include "Countable.h"
#include "Money.h"
#include "StockExchange.h"

void TurnManager::init()
{
	Countable i;
	Money m;
	BuyingAsk ask(i, 10);
	BuyingAsk ask2(m, 20);

	std::vector<size_t> keys;
	keys.resize(2);
	keys[0] = typeid(Countable).hash_code();
	keys[1] = typeid(Money).hash_code();

	StockExchange bourse(keys);
	bourse.registerBuyingAsk(ask);
	bourse.registerBuyingAsk(ask2);

	bourse.resolveOffers();
	
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

