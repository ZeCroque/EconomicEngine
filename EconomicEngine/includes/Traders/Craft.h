#ifndef CRAFT_H
#define CRAFT_H

#include "Clonable.h"

#include <list>

#include "Signal.h"

class Craft final: public Clonable<Craft>
{
private:
	float elapsedTime;
	float rate;
	int count;
	size_t result;
	std::list<std::pair<size_t, int>> requirements;
	std::list<size_t> toolsRequired;

	Signal<> craftSuccessSignal;

public:
	Craft();
	Craft(float baseRate, size_t craftResult, int count = 1, std::list<std::pair<size_t, int>> requirements = std::list<std::pair<size_t, int>>(), std::list<size_t> toolsRequired = std::list<size_t>());
	Craft(Craft& craft);
	Craft* clone() override;

    [[nodiscard]] std::list<std::pair<size_t, int>> getRequirement() const;
	[[nodiscard]] std::list<size_t> getToolsRequired() const;
	[[nodiscard]] size_t getResult() const;
	[[nodiscard]] int getCount() const;
	[[nodiscard]] float getRate() const;
	[[nodiscard]] const Signal<>& getCraftSuccessSignal() const;
	void incrementRate(float i);

	void update(float deltaTime);

};




#endif
