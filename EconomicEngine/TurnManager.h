#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H
#include "Observable.h"
#include "Singleton.h"

class TurnManager final : public Observable, public Singleton<TurnManager>
{
	friend class Singleton<TurnManager>;
	private:
		bool bRunning;
		TurnManager() : bRunning(false){}
		~TurnManager() = default;


	public:	
		void init();
		void reset();
		int exec();
		void stop();
};

#endif