#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H
#include "Observable.h"
#include "Singleton.h"
#include "StockExchange.h"
#include "TradableManager.h"
#include "TraderManager.h"
#include <mutex>
#include <thread>

template <class T> class EconomicEngine : public Observable, public Singleton<T>
{
protected:
	TraderManager* traderManager;
	TradableManager* tradableManager;
	StockExchange* stockExchange;

	EconomicEngine() : traderManager(TraderManager::getInstance()), tradableManager(TradableManager::getInstance()), stockExchange(StockExchange::getInstance()), bRunning(false), bPaused(true), turnSecond(1), step(1) {}
	virtual ~EconomicEngine() = default;
private:
	bool bRunning;
	bool bPaused;
	int turnSecond;
	int step;
	std::condition_variable cv;
	std::mutex m;

public:

	virtual void initTraders() const = 0;
	virtual void initTradables() const = 0;
	
	void init() const
	{
		initTraders();
		initTradables();
		stockExchange->init();
	}

	void reset(const int count) const
	{
		traderManager->reset();
		stockExchange->reset();
		traderManager->addTrader(count);
	}

	int exec(const int count)
	{
		this->bRunning = true;
		auto pauseTime = 250;

		//Create traders
		traderManager->addTrader(count);

		while (bRunning)
		{
			while (bPaused)
			{
				std::unique_lock<std::mutex> lk(m);
				cv.wait(lk);
				lk.unlock();
			}
			for (auto i = 0; i < this->step; i++)
			{
				stockExchange->incrementTurnCount();
				traderManager->doTradersCrafting();
				traderManager->doTradersAsking();
				stockExchange->resolveOffers();
				traderManager->refreshTraders();
				traderManager->killTraders();
			}

			this->notifyObservers();
			pauseTime = 1000 / turnSecond;
			std::this_thread::sleep_for(std::chrono::milliseconds(pauseTime));
		}
		return 0;
	}

	void stop()
	{
		if(bPaused)
		{
			resume();
		}
		this->bRunning = false;
	}

	void pause()
	{
		std::lock_guard<std::mutex> lk(m);
		this->bPaused = true;
	}

	void resume()
	{
		std::lock_guard<std::mutex> lk(m);
		bPaused = false;
		cv.notify_one();
	}

	void setTurnSecond(const int turnSecond)
	{
		this->turnSecond = turnSecond;
	}

	[[nodiscard]] int getTurnCount() const
	{
		return stockExchange->getTurnCount();
	}

	void setStep(const int step)
	{
		this->step = step;
	}

};



#endif
