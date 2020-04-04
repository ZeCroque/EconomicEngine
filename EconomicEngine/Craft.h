#ifndef CRAFT_H
#define CRAFT_H
#include <map>
#include <vector>


#include "Clonable.h"
#include "Tradable.h"

//TODO reuse abstract factory

class Craft : public Clonable<Craft>
{
private:
	float advancement;
	float baseRate;
	float currentRate;
	size_t craftResult;
	std::vector<std::pair<size_t, int>> requirements;
public:
	Craft();
	Craft(float baseRate, size_t craftResult, std::vector<std::pair<size_t, int>> requirements);
	Craft(Craft& craft);
	Craft* clone() override;

	[[nodiscard]] std::vector<std::pair<size_t, int>> getRequirement() const;
	Tradable advanceCraft();

};




#endif
