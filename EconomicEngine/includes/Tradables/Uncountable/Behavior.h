#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <string>

#include "Clonable.h"

class Tradable;

class Behavior : public Clonable<Behavior>
{
protected:
	class Uncountable* owningTool;
	size_t id;
public:
	Behavior();
	explicit Behavior(const std::string& inName);
	virtual void init(Uncountable* inOwningTool) = 0;
	[[nodiscard]] size_t getId() const;

};
#endif
