#include "Craft.h"

#include <utility>

#include "TradableManager.h"

Craft::Craft() : advancement(0.0f), rate(0.0f), count(), result(0){}

Craft::Craft(const float baseRate, const size_t craftResult, const int count, std::list<std::pair<size_t, int>> requirements, std::list<size_t> toolsRequired) : advancement(0.0f), rate(baseRate), count(count), result(craftResult), requirements(std::move(requirements)), toolsRequired(std::move(toolsRequired)){}
	                                                                                                                           
Craft::Craft(Craft& craft) : advancement(craft.advancement), rate(craft.rate), count(craft.count), result(craft.result), requirements(craft.requirements), toolsRequired(craft.toolsRequired) {}

Craft* Craft::clone()
{
	return new Craft(*this);
}

std::list<std::pair<size_t, int>> Craft::getRequirement() const
{
	return requirements;
}

std::list<size_t> Craft::getToolsRequired() const
{
	return toolsRequired;
}

size_t Craft::getResult() const
{
	return result;
}

int Craft::getCount() const
{
	return count;
}

float Craft::getRate() const
{
	return rate;
}

void Craft::incrementRate(const float i)
{
	rate += i;
}

void Craft::decrementRate(const float i)
{
	rate-=i;
}

bool Craft::advanceCraft()
{
	bool result = false;
	advancement += rate;
	if(advancement >= 1.0f)
	{
		result = true;
		notifyObservers();
	}
	return result;
}
