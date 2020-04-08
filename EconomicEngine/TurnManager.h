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
	int turnSecond;
	int turnNumber;
	TraderManager* traderManager;
	TradableManager* tradableManager;
	StockExchange* stockExchange;


	TurnManager();
	~TurnManager() = default;


public:
	void init() const;
	void reset();
	int exec();
	void stop();

	void setTurnSecond(int turnSecond);
	int getTurnNumber() const;
	void setTurnNumber(int turnNumber);
};

#endif
