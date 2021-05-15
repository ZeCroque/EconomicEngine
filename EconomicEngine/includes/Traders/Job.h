#ifndef JOB_H
#define JOB_H

#include <string>

#include "CraftFactory.h"
#include "Craft.h"

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
	explicit Job(std::string inName);
	Job(const Job& job);
	~Job() override;
	void setOwner(Trader* inOwner) const;
	[[nodiscard]] Craft* createCraft(size_t inId) const;
	[[nodiscard]] std::vector<size_t> getCraftList() const;
	[[nodiscard]] std::vector<size_t> getCraftableList() const;
	[[nodiscard]] std::vector<size_t> getUncraftableList() const;

	[[maybe_unused]] [[nodiscard]] const std::list<size_t>& getUsableTools() const;
	[[nodiscard]] std::list<size_t>& getUsableTools();
	[[nodiscard]] Craft* getCraft(size_t inKey) const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] CraftFactory* getCraftFactory() const;
	Job* clone() override { return new Job(*this); }
};

#endif //JOB_H