#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H
#include "Observable.h"
#include "Singleton.h"
#include "StockExchange.h"
#include "TradableManager.h"
#include "TraderManager.h"

class TurnManager final : public Observable, public Singleton<TurnManager>
{
	friend class Singleton<TurnManager>;
private:
	bool bRunning;
	bool bPaused;
	int turnSecond;
	int turnNumber;
	int step;
	TraderManager* traderManager;
	TradableManager* tradableManager;
	StockExchange* stockExchange;

	TurnManager();
	~TurnManager() = default;


public:
	void init() const;
	void reset(int count);
	int exec(int count);
	void stop();
	
	void pause();
	void resume();
	void setTurnSecond(int turnSecond);
	[[nodiscard]] int getTurnNumber() const;
	void setTurnNumber(int turnNumber);
	void setStep(int step);
};

#endif
