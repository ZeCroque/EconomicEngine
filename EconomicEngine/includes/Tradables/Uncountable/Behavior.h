#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "Clonable.h"

class Tradable;

class Behavior : public Clonable<Behavior>
{
protected:
	class Uncountable* owningTool;
	size_t typeId;
public:
	Behavior();
	virtual ~Behavior() = default;
	virtual void init(Uncountable* owner) = 0;
	[[nodiscard]] size_t getId() const;

};
#endif
