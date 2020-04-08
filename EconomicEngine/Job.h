#ifndef JOB_H
#define JOB_H

#include "CraftFactory.h"
#include "Craft.h"

class Trader;
class CraftFactory;

class Job : public Clonable<Job>
{
protected:
	CraftFactory* craftFactory;
public:
	Job();
	Job(const Job& job);
	virtual ~Job();
	void setOwner(Trader* owner) const;
	[[nodiscard]] Craft* craft(size_t typeId) const;
	[[nodiscard]] std::vector<size_t> getCraftableList() const;
	[[nodiscard]] std::vector<size_t> getUncraftableList() const;
	[[nodiscard]] Craft* getCraft(size_t key) const;
};

class Miner : public Job
{
public:
	Miner();
	Miner* clone() override;
};

#endif