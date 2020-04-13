#ifndef DEBUG_TURN_MANAGER
#define DEBUG_TURN_MANAGER
#include "TurnManager.h"

class DebugTurnManager : public TurnManager<DebugTurnManager>
{
	friend class Singleton<DebugTurnManager>;

	DebugTurnManager();
	void initTraders() const override;
	void initTradables() const override;
};

#endif
