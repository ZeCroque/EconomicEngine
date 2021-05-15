#include "Traders/Craft.h"
#include "Traders/Trader.h"

Craft::Craft() : elapsedTime(0.0f), craftingRate(0.0f), producedAmount(), resultId(0)
{
}

Craft::Craft(const float inBaseRate, const size_t inCraftResult, const int inProducedAmount,
std::list<std::pair<size_t, int>> inRequirements, std::list<size_t> inToolsRequired) : elapsedTime(0.0f),
																						craftingRate(inBaseRate), producedAmount(inProducedAmount), resultId(inCraftResult),
																						requirements(std::move(inRequirements)), toolsRequired(std::move(inToolsRequired))
{
}
	                                                                                                                           
Craft::Craft(Craft& craft) : elapsedTime(craft.elapsedTime), craftingRate(craft.craftingRate),
                             producedAmount(craft.producedAmount), resultId(craft.resultId),
                             requirements(craft.requirements), toolsRequired(craft.toolsRequired)
{
}

void Craft::update(const float inDeltaTime)
{
	elapsedTime += inDeltaTime;
	if(elapsedTime >= EconomicEngine::getInstance()->getBaseActionTime() * craftingRate)
	{
		elapsedTime = 0.f;
		craftSuccessSignal();
	}
}

std::list<std::pair<size_t, int>> Craft::getRequirements() const
{
	return requirements;
}

std::list<size_t> Craft::getRequiredTools() const
{
	return toolsRequired;
}

size_t Craft::getResultId() const
{
	return resultId;
}

int Craft::getCount() const
{
	return producedAmount;
}

float Craft::getCraftingRate() const
{
	return craftingRate;
}

const Signal<>& Craft::getCraftSuccessSignal() const
{
	return craftSuccessSignal;
}

void Craft::incrementCraftingRate(const float inAmount)
{
	craftingRate += inAmount;
}

Craft* Craft::clone()
{
	return new Craft(*this);
}

			
			