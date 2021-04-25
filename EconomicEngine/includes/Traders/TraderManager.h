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
	void registerJob(Job* job);
	void addTrader(int count);
	void addTrader(int count, size_t key);
    const Signal<Trader*>& getTraderAddedSignal() const;

	[[nodiscard]] std::list<std::pair<size_t, std::string>> getJobList() const;
	[[nodiscard]] std::list<const Trader*> getTraderByJobId(size_t key) const;
	[[nodiscard]] std::pair<int, int> getDemographyByJob(size_t key) const;
	[[nodiscard]] float getMoneyMeanByJob(size_t key) const;
	[[nodiscard]] float getFoodLevelMeanByJob(size_t key) const;
	[[nodiscard]] int getJobCount(size_t key) const;
	[[nodiscard]] size_t getMostInterestingJob() const;
	void killStarvedTraders();
	void clearPendingKillTraders();
	void makeChildren();
	void update(float deltaTime);
	void reset();
	void markForKill(size_t key, int count);
};

#endif

