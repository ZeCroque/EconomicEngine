#ifndef JOB_H
#define JOB_H

#include "CraftFactory.h"
#include "Craft.h"
#include "Trader.h"

class Trader;
class CraftFactory;

class Job
{
private:
	CraftFactory* craftFactory;
public:
	Job();
	Job(Trader& owner);
	~Job();
	[[nodiscard]] Craft* craft(size_t typeId) const;
	[[nodiscard]] std::vector<size_t> getCraftableList() const;
	virtual void init() = 0;
};

#endif