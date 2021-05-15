#ifndef ECONOMIC_ENGINE_H
#define ECONOMIC_ENGINE_H

#include <nlohmann/json.hpp>

#include "Singleton.h"
#include "StockExchange/StockExchange.h"
#include "Tradables/TradableFactory.h"
#include "Traders/TraderManager.h"

class EconomicEngine : public Singleton<EconomicEngine>
{
friend class Singleton<EconomicEngine>;

//FRAMEWORK
//===========

public:
	void init(const char* prefabsPath) const;

	void start(int inTradersAmount);

	void pause();

	void resume();
	
	void reset(int inTradersAmount);

	void update(float inDeltaTime);

private:
	EconomicEngine();

	void initJobs(std::vector<nlohmann::json>& inParsedJobs) const;
	
	void initTradables(std::vector<nlohmann::json>& inParsedTradables) const;

	bool bRunning;

//GAMEPLAY
//===========
public:
	[[nodiscard]] float getBaseActionTime() const;

	[[nodiscard]] int getElapsedDayCount() const;

	[[nodiscard]] TradableFactory& getTradableFactory() const;

	[[nodiscard]] TraderManager& getTraderManager() const;

	[[nodiscard]] StockExchange& getStockExchange() const;

private:
	
	mutable TraderManager traderManager;
	mutable TradableFactory tradableFactory;
	mutable StockExchange stockExchange;
	
	int elapsedDayCount;
	float elapsedTimeSinceDayStart;
	float dayDuration;
	float elapsedTimeSinceLastStockExchangeResolution;
	float stockExchangeResolutionTime;
	float baseActionTime;
};


#endif //ECONOMIC_ENGINE
