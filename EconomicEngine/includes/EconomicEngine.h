#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include "Singleton.h"
#include "nlohmann/json.hpp"
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

	EconomicEngine();
private:
	bool bRunning;

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
