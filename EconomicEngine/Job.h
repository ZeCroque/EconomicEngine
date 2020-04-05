#ifndef JOB_H
#define JOB_H

#include "CraftFactory.h"
#include "Craft.h"
#include "Trader.h"

class Trader;
class CraftFactory;

class Job : public Clonable<Job>
{
private:
	CraftFactory* craftFactory;
public:
	Job();
	Job(const Job& job);
	~Job();
	void setOwner(Trader* owner);
	[[nodiscard]] Craft* craft(size_t typeId) const;
	[[nodiscard]] std::vector<size_t> getCraftableList() const;
	Job* clone() override;
	virtual void init();
};

#endif