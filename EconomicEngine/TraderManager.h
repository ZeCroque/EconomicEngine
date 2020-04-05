#ifndef TRADER_MANAGER_H
#define TRADER_MANAGER_H
#include "JobFactory.h"
#include "Singleton.h"
#include "Trader.h"

class TraderManager final : public Singleton<TraderManager>
{
private:
	std::list<Trader> traders;
	JobFactory jobFactory;
public:
	void registerJob(Job* job);
	void addTrader(int count);
	[[nodiscard]] Job* assignJob(size_t key, Trader* trader) const;
	size_t getMostInterestingJob();
	void refreshTraders();
};

#endif

