#ifndef TURNMANAGER__H
#define TURNMANAGER__H

class TurnManager
{

private:
	static TurnManager* _instance;
	TurnManager() = default;
	~TurnManager() = default;
	 

public:
	static TurnManager& GetInstance();
	TurnManager(const TurnManager&) = delete;
	TurnManager& operator = (const TurnManager &) = delete;
};

#endif