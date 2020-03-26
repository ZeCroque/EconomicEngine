#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H
#include "Observable.h"

class TurnManager final : public Observable
{

	private:
		static TurnManager* _instance;
		bool bRunning;
		TurnManager() : bRunning(false){}
		~TurnManager() = default;
		 

	public:
		static TurnManager& getInstance();
		TurnManager(const TurnManager&) = delete;
		TurnManager& operator = (const TurnManager &) = delete;
		TurnManager(const TurnManager&&) = delete;
		TurnManager& operator = (const TurnManager&&) = delete;
		int exec();
		void stop();
};

#endif