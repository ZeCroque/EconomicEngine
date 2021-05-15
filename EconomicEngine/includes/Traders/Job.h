#ifndef JOB_H
#define JOB_H

#include <string>

#include "CraftFactory.h"
#include "Craft.h"

class Trader;

class Job : public Clonable<Job>
{
public:
	Job();
	
	explicit Job(std::string inName);
	
	Job(const Job& job);
	
	~Job() override;

	void addUsableTool(size_t toolId);
	
	[[nodiscard]] Craft* createCraft(size_t inCraftId) const;
	
	[[nodiscard]] std::vector<size_t> getCraftsIds() const;
	
	[[nodiscard]] std::vector<size_t> getCraftablesIds() const;
	
	[[nodiscard]] std::vector<size_t> getUncraftablesIds() const;

	[[nodiscard]] const std::list<size_t>& getUsableTools() const;
	
	[[nodiscard]] Craft* getCraft(size_t inCraftId) const;
	
	[[nodiscard]] size_t getId() const;
	
	[[nodiscard]] std::string getName() const;
	
	[[nodiscard]] CraftFactory* getCraftFactory() const;

	void setOwner(Trader* inOwner) const;
	
	Job* clone() override;

private:
	CraftFactory* craftFactory;
	std::list<size_t> usableTools;
	size_t id;
	std::string name;
};

#endif //JOB_H