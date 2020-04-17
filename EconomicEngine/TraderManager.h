#ifndef TRADER_MANAGER_H
#define TRADER_MANAGER_H
#include "JobFactory.h"
#include "Singleton.h"
#include "Trader.h"

class TraderManager final : public Singleton<TraderManager>
{
private:
	std::list<Trader> traders;
	mutable VectorArray < std::pair<int, int>> demographyCounts;
	JobFactory jobFactory;

	
public:
	void init() const;
	void registerJob(Job* job);
	void addTrader(int count);
	void addTrader(int count, size_t key);
	[[nodiscard]] Job* assignJob(size_t key, Trader* trader) const;
	[[nodiscard]] std::list<std::pair<size_t, std::string>> getJobList() const;
	[[nodiscard]] std::list<Trader*> getTraderByJobId(size_t key);
	[[nodiscard]] std::pair<int, int> getDemographyByJob(size_t key) const;
	[[nodiscard]] float getMoneyMeanByJob(size_t key);
	[[nodiscard]] float getFoodLevelMeanByJob(size_t key);
	[[nodiscard]] int getJobCount(size_t key);
	[[nodiscard]] size_t getMostInterestingJob();
	void refreshTraders();
	void killTraders();
	void doTradersCrafting();
	void doTradersAsking();
	void reset();
	void kill(size_t key, int count);
};

#endif

