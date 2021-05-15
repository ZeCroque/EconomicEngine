#ifndef TRADER_MANAGER_H
#define TRADER_MANAGER_H

#include "JobFactory.h"
#include "Signal.h"
#include "VectorArray.h"

class TraderManager final
{

private:
	std::list<Trader> traders;
	std::list<Trader*> pendingKillTraders;
	mutable VectorArray< std::pair<int, int>> demographyCounts;
	JobFactory jobFactory;
    Signal<Trader*> traderAddedSignal;

public:
	void init() const;
	void registerJob(Job* inJob);
	void addTrader(int inCount);
	void addTrader(int inCount, size_t inKey);
    const Signal<Trader*>& getTraderAddedSignal() const;

	[[nodiscard]] std::list<std::pair<size_t, std::string>> getJobList() const;
	[[nodiscard]] std::list<const Trader*> getTraderByJobId(size_t inKey) const;
	[[nodiscard]] std::pair<int, int> getDemographyByJob(size_t inKey) const;
	[[nodiscard]] float getMoneyMeanByJob(size_t inKey) const;
	[[nodiscard]] float getFoodLevelMeanByJob(size_t inKey) const;
	[[nodiscard]] int getJobCount(size_t inKey) const;
	[[nodiscard]] size_t getMostInterestingJob() const;
	void killStarvedTraders();
	void clearPendingKillTraders();
	void makeChildren();
	void update(float inDeltaTime);
	void reset();
	void markForKill(size_t inKey, int inCount);
};

#endif //TRADER_MANAGER_H

