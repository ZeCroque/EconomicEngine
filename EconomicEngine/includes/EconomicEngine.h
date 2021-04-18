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
	TraderManager* traderManager;
	mutable TradableFactory tradableFactory;
	StockExchange* stockExchange;

	EconomicEngine() : traderManager(TraderManager::getInstance()), stockExchange(StockExchange::getInstance()), bRunning(false), bPaused(true), turnSecond(1), step(1), elapsedTimeSinceDayStart(0), dayDuration(320.f), baseActionTime(dayDuration / 4.f)  {}
private:
	bool bRunning;
	bool bPaused;
	int turnSecond;
	int step;
	Signal<> asksResolvedSignal;
	float elapsedTimeSinceDayStart;
	float dayDuration;
	float baseActionTime;

public:

	const Signal<>& getAsksResolvedSignal() const;
	
	void initJobs(std::vector<nlohmann::json>& parsedJobs) const;
	
	void initTradables(std::vector<nlohmann::json>& parsedTradables) const;
	
	void init(const char* prefabsPath) const;

	void start(int count);
	
	void update(float deltaTime);
	
	void reset(int count);

	void pause();

	void resume();

	void setTurnSecond(const int turnSecond);

	[[nodiscard]] int getTurnCount() const;

	void setStep(const int step);

	[[nodiscard]] float getBaseActionTime() const;

	[[nodiscard]] const TradableFactory& getTradableFactory() const;

};


#endif
