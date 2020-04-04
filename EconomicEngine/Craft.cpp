#include "Craft.h"

#include <utility>

Craft::Craft() : advancement(0.0f), baseRate(0.0f), currentRate(0.0f), craftResult(0){}

Craft::Craft(float baseRate, size_t craftResult, std::vector<std::pair<size_t, int>> requirements) : advancement(0.0f), baseRate(baseRate), currentRate(baseRate), craftResult(craftResult), requirements(std::move(requirements)) { }
	                                                                                                                           

Craft::Craft(Craft& craft) : advancement(craft.advancement), baseRate(craft.baseRate), currentRate(craft.currentRate), craftResult(craft.craftResult), requirements(craft.requirements) {}

Craft* Craft::clone()
{
	return new Craft(*this);
}

std::vector<std::pair<size_t, int>> Craft::getRequirement() const
{
	return requirements;
}
