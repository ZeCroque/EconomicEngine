#ifndef DEBUG_TURN_MANAGER
#define DEBUG_TURN_MANAGER
#include "EconomicEngine.h"

class DebugEconomicEngine : public EconomicEngine<DebugEconomicEngine>
{
	friend class Singleton<DebugEconomicEngine>;

	DebugEconomicEngine();
	void initTraders() const override;
	void initTradables() const override;
};

#endif
