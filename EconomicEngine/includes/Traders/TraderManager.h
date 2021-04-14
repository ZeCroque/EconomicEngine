#ifndef TRADER_MANAGER_H
#define TRADER_MANAGER_H

#include "JobFactory.h"
#include "Singleton.h"
#include "Trader.h"
#include "Signal.h"
#include <any>

class TraderManager final : public Singleton<TraderManager>
{
private:
	std::list<Trader> traders;
	mutable VectorArray < std::pair<int, int>> demographyCounts;
	JobFactory jobFactory;
    Signal<Trader*> addTraderSignal;
    Signal<Trader*> killTraderSignal;

public:
	void init() const;
	void registerJob(Job* job);
	void addTrader(int count);
	void addTrader(int count, size_t key);
    const Signal<Trader*>& getAddTraderSignal() const;
    const Signal<Trader*>& getKillTraderSignal() const;

    [[maybe_unused]] [[nodiscard]] Job* assignJob(size_t key, Trader* trader) const;
	[[nodiscard]] std::list<std::pair<size_t, std::string>> getJobList() const;
	[[nodiscard]] std::list<const Trader*> getTraderByJobId(size_t key) const;
	[[nodiscard]] std::pair<int, int> getDemographyByJob(size_t key) const;
	[[nodiscard]] float getMoneyMeanByJob(size_t key) const;
	[[nodiscard]] float getFoodLevelMeanByJob(size_t key) const;
	[[nodiscard]] int getJobCount(size_t key) const;
	[[nodiscard]] size_t getMostInterestingJob() const;
	void refreshTraders();
	void killTraders();
	void doTradersCrafting();
	void doTradersAsking();
	void reset();
	void kill(size_t key, int count);
};

#endif

