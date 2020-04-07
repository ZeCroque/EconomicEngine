#ifndef TRADABLE_H
#define TRADABLE_H
#include <string>

#include "Behavior.h"
#include "Clonable.h"

class Tradable : public Clonable<Tradable>
{
protected:
	size_t id;
	std::string name;
	std::pair<float, float> defaultPriceBelief;
	Behavior* behavior;

public:
	Tradable();
	Tradable(const Tradable& tradable);
	virtual ~Tradable() = default;

	[[nodiscard]] std::string getName() const;
	[[nodiscard]] size_t getId() const;
	[[nodiscard]] std::pair<float, float> getDefaultPriceBelief() const;
};
#endif
