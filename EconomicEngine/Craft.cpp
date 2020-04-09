#include "Craft.h"

#include <utility>

#include "TradableManager.h"

Craft::Craft() : advancement(0.0f), rate(0.0f), result(0){}

Craft::Craft(const float baseRate, const size_t craftResult, std::vector<std::pair<size_t, int>> requirements, std::vector<size_t> toolsRequired) : advancement(0.0f), rate(baseRate), result(craftResult), requirements(std::move(requirements)), toolsRequired(std::move(toolsRequired)){}
	                                                                                                                           

Craft::Craft(Craft& craft) : advancement(craft.advancement), rate(craft.rate), result(craft.result), requirements(craft.requirements), toolsRequired(craft.toolsRequired) {}

Craft* Craft::clone()
{
	return new Craft(*this);
}

std::vector<std::pair<size_t, int>> Craft::getRequirement() const
{
	return requirements;
}

std::vector<size_t> Craft::getToolsRequired() const
{
	return toolsRequired;
}

size_t Craft::getResult() const
{
	return result;
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

Tradable* Craft::advanceCraft()
{
	Tradable* result = nullptr;
	advancement += rate;
	if(advancement >= 1.0f)
	{
		result = TradableManager::getInstance()->createTradable(this->result);
		notifyObservers();
	}
	return result;
}
