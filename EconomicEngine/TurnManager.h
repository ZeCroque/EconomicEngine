#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

class TurnManager
{

private:
	static TurnManager* _instance;
	TurnManager() = default;
	~TurnManager() = default;
	 

public:
	static TurnManager& getInstance();
	TurnManager(const TurnManager&) = delete;
	TurnManager& operator = (const TurnManager &) = delete;
	TurnManager(const TurnManager&&) = delete;
	TurnManager& operator = (const TurnManager&&) = delete;
};

#endif