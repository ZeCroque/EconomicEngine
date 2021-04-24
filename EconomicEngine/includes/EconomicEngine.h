#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include "Singleton.h"
#include "nlohmann/json.hpp"
#include "Signal.h"
#include "StockExchange/StockExchange.h"
#include "Tradables/TradableFactory.h"
#include "Traders/TraderManager.h"


class EconomicEngine : public Singleton<EconomicEngine>
{
friend class Singleton<EconomicEngine>;
	
protected:
	mutable TraderManager traderManager;
	mutable TradableFactory tradableFactory;
	mutable StockExchange stockExchange;

	EconomicEngine() : bRunning(false), bPaused(true), elapsedDayCount(0), elapsedTimeSinceDayStart(0), dayDuration(320.f), elapsedTimeSinceLastStockExchangeResolution(0.f),stockExchangeResolutionTime(dayDuration / 100.f),baseActionTime(dayDuration / 200.f)  {}
private:
	bool bRunning;
	bool bPaused;

	int elapsedDayCount;
	float elapsedTimeSinceDayStart;
	float dayDuration;
	float elapsedTimeSinceLastStockExchangeResolution;
	float stockExchangeResolutionTime;
	float baseActionTime;

public:
	
	void initJobs(std::vector<nlohmann::json>& parsedJobs) const;
	
	void initTradables(std::vector<nlohmann::json>& parsedTradables) const;
	
	void init(const char* prefabsPath) const;

	void start(int count);
	
	void update(float deltaTime);
	
	void reset(int count);

	void pause();

	void resume();

	[[nodiscard]] float getBaseActionTime() const;

	[[nodiscard]] int getElapsedDayCount() const;

	[[nodiscard]] TradableFactory& getTradableFactory() const;

	[[nodiscard]] TraderManager& getTraderManager() const;

	[[nodiscard]] StockExchange& getStockExchange() const;

};


#endif
