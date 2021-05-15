#ifndef TRADER_MANAGER_H
#define TRADER_MANAGER_H

#include "JobFactory.h"
#include "Signal.h"
#include "VectorArray.h"

class TraderManager final
{
public:
	void init() const;

	void reset();

	void update(float inDeltaTime);
	
	void registerJob(Job* inJob);
	
	void addTraders(int inAmount);
	
	void addTraders(int inAmount, size_t jobId);
	
	void killStarvedTraders();

	void markForKill(size_t inJobId, int inAmount);
	
	void clearPendingKillTraders();
	
	void makeChildren();
	
	[[nodiscard]] std::list<std::pair<size_t, std::string>> getJobsIds() const;

	[[nodiscard]] std::pair<int, int> getDemographyByJob(size_t inJobId) const;
	
	[[nodiscard]] std::list<const Trader*> getTraderByJob(size_t inJobId) const;
	
	[[nodiscard]] float getMoneyMeanByJob(size_t inJobId) const;
	
	[[nodiscard]] float getFoodLevelMeanByJob(size_t inJobId) const;
	
	[[nodiscard]] int getTraderCountByJob(size_t inJobId) const;
	
	[[nodiscard]] size_t getMostInterestingJobId() const;

    const Signal<Trader*>& getTraderAddedSignal() const;

private:
	std::list<Trader> traders;
	std::list<Trader*> pendingKillTraders;
	mutable VectorArray< std::pair<int, int>> demographyCounts;
	JobFactory jobFactory;
    Signal<Trader*> traderAddedSignal;

};

#endif //TRADER_MANAGER_H

