#ifndef CRAFT_H
#define CRAFT_H

#include <vector>
#include "Clonable.h"
#include "Observable.h"

class Craft : public Clonable<Craft>, public Observable
{
private:
	float advancement;
	float rate;
	int count;
	size_t result;
	std::vector<std::pair<size_t, int>> requirements;
	std::vector<size_t> toolsRequired;
public:
	Craft();
	Craft(float baseRate, size_t craftResult, int count = 1, std::vector<std::pair<size_t, int>> requirements = std::vector<std::pair<size_t, int>>(), std::vector<size_t> toolsRequired = std::vector<size_t>());
	Craft(Craft& craft);
	Craft* clone() override;

	[[nodiscard]] std::vector<std::pair<size_t, int>> getRequirement() const;
	[[nodiscard]] std::vector<size_t> getToolsRequired() const;
	[[nodiscard]] size_t getResult() const;
	[[nodiscard]] int getCount() const;
	[[nodiscard]] float getRate() const;
	void incrementRate(float i);
	void decrementRate(float i);
	bool advanceCraft();

};




#endif
