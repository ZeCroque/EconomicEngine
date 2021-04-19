#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include "Singleton.h"
#include "nlohmann/json.hpp"
#include <mutex>

#include "Signal.h"
#include "StockExchange/StockExchange.h"
#include "Tradables/TradableManager.h"
#include "Traders/TraderManager.h"


class EconomicEngine : public Singleton<EconomicEngine>
{
friend class Singleton<EconomicEngine>;
	
protected:
	TraderManager* traderManager;
	TradableManager* tradableManager;
	StockExchange* stockExchange;

	EconomicEngine() : traderManager(TraderManager::getInstance()), tradableManager(TradableManager::getInstance()), stockExchange(StockExchange::getInstance()), bRunning(false), bPaused(true), turnSecond(1), step(1) {}
private:
	bool bRunning;
	bool bPaused;
	int turnSecond;
	int step;
	std::condition_variable cv;
	std::mutex m;
	Signal<> postInitSignal;
	Signal<> asksResolvedSignal;

public:

	const Signal<>& getAsksResolvedSignal() const;
	
	const Signal<>& getPostInitSignal() const;
	
	void initJobs(std::vector<nlohmann::json>& parsedJobs) const;
	
	void initTradables(std::vector<nlohmann::json>& parsedTradables) const;
	
	void init(const char* prefabsPath) const;

	void reset(const int count) const;

	int exec(const int count);

	void stop();

	void pause();

	void resume();

	void setTurnSecond(const int turnSecond);

	[[nodiscard]] int getTurnCount() const;

	void setStep(const int step);

};


#endif
