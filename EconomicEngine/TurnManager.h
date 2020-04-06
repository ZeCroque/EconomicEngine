#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H
#include "Observable.h"
#include "Singleton.h"
#include "TradableManager.h"
#include "TraderManager.h"

class TurnManager final : public Observable, public Singleton<TurnManager>
{
	friend class Singleton<TurnManager>;
	private:
		bool bRunning;
		TraderManager* traderManager;
		TradableManager* tradableManager;
	
		TurnManager(); 
		~TurnManager() = default;


	public:	
		void init() const;
		void reset();
		int exec();
		void stop();
};

#endif