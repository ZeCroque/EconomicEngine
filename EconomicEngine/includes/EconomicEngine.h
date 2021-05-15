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
	
	void initJobs(std::vector<nlohmann::json>& inParsedJobs) const;
	
	void initTradables(std::vector<nlohmann::json>& inParsedTradables) const;
	
	void init(const char* prefabsPath) const;

	void start(int inCount);
	
	void update(float inDeltaTime);
	
	void reset(int inCount);

	void pause();

	void resume();

	[[nodiscard]] float getBaseActionTime() const;

	[[nodiscard]] int getElapsedDayCount() const;

	[[nodiscard]] TradableFactory& getTradableFactory() const;

	[[nodiscard]] TraderManager& getTraderManager() const;

	[[nodiscard]] StockExchange& getStockExchange() const;

};


#endif //ECONOMIC_ENGINE
