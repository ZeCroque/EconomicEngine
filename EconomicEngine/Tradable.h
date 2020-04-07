#ifndef TRADABLE_H
#define TRADABLE_H
#include <string>

#include "Behavior.h"
#include "Clonable.h"

class Tradable : public Clonable<Tradable>
{
private:
	std::string name;
	Behavior* behavior;

protected:
	size_t id;
	
public:
	Tradable();
	Tradable(const Tradable& tradable);
	virtual ~Tradable() = default;

	[[nodiscard]] std::string getName() const;
	[[nodiscard]] size_t getId() const;
};

#endif
