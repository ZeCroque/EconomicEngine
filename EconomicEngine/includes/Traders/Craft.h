#ifndef CRAFT_H
#define CRAFT_H

#include <list>

#include "Clonable.h"
#include "Signal.h"

class Craft final: public Clonable<Craft>
{
public:
	Craft();
	
	Craft(float inBaseRate, size_t inCraftResult, int inProducedAmount = 1,
	      std::list<std::pair<size_t, int>> inRequirements = std::list<std::pair<size_t, int>>(),
	      std::list<size_t> inToolsRequired = std::list<size_t>());

	Craft(Craft& craft);

	void update(float inDeltaTime);
	
    [[nodiscard]] std::list<std::pair<size_t, int>> getRequirements() const;
	
	[[nodiscard]] std::list<size_t> getRequiredTools() const;
	
	[[nodiscard]] size_t getResultId() const;
	
	[[nodiscard]] int getCount() const;
	
	[[nodiscard]] float getCraftingRate() const;
	
	[[nodiscard]] const Signal<>& getCraftSuccessSignal() const;
	
	void incrementCraftingRate(float inAmount);

	Craft* clone() override;

private:
	float elapsedTime;
	float craftingRate;
	int producedAmount;
	size_t resultId;
	std::list<std::pair<size_t, int>> requirements;
	std::list<size_t> toolsRequired;

	Signal<> craftSuccessSignal;
};

#endif //CRAFT_H
