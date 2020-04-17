#ifndef JOB_H
#define JOB_H

#include "CraftFactory.h"
#include "Craft.h"

#include <string>

class Trader;

class Job : public Clonable<Job>
{
protected:
	CraftFactory* craftFactory;
	std::list<size_t> usableToolsList;
	size_t id;
	std::string name;
public:
	Job();
	Job(const Job& job);
	virtual ~Job();
	void setOwner(Trader* owner) const;
	[[nodiscard]] Craft* createCraft(size_t typeId) const;
	[[nodiscard]] std::vector<size_t> getCraftList() const;
	[[nodiscard]] std::vector<size_t> getCraftableList() const;
	[[nodiscard]] std::vector<size_t> getUncraftableList() const;
	[[nodiscard]] std::list<size_t> getUsableTools() const;
	[[nodiscard]] Craft* getCraft(size_t key) const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] std::string getName() const;
};

#endif