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
	[[nodiscard]] std::list<std::pair<size_t, std::string>> getJobList() const;
	[[nodiscard]] int getJobCount(size_t key);
	[[nodiscard]] size_t getMostInterestingJob();
	void refreshTraders();
	void doTradersCrafting();
	void doTradersAsking();
	void reset();
	void kill(size_t key, int count);
};

#endif

